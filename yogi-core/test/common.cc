#include "common.h"
#include <boost/uuid/uuid_io.hpp>

Test::Test() { SetupLogging(YOGI_VB_TRACE); }

Test::~Test() {
  EXPECT_EQ(YOGI_DestroyAll(), YOGI_OK);

  YOGI_LogToConsole(YOGI_VB_NONE, 0, 0, nullptr, nullptr);
  YOGI_LogToHook(YOGI_VB_NONE, nullptr, nullptr);
  YOGI_LogToFile(YOGI_VB_NONE, nullptr, nullptr, 0, nullptr, nullptr);
}

BranchEventRecorder::BranchEventRecorder(void* context, void* branch)
    : context_(context), branch_(branch), json_str_(10000) {
  StartAwaitEvent();
}

nlohmann::json BranchEventRecorder::RunContextUntil(
    int event, const boost::uuids::uuid& uuid, int ev_res) {
  while (true) {
    for (auto& entry : events_) {
      if (entry.uuid == uuid && entry.event == event &&
          entry.ev_res == ev_res) {
        return entry.json;
      }
    }

    auto n = events_.size();
    while (n == events_.size()) {
      int res = YOGI_ContextRunOne(context_, nullptr, -1);
      EXPECT_EQ(res, YOGI_OK);
    }
  }
}

nlohmann::json BranchEventRecorder::RunContextUntil(int event, void* branch,
                                                    int ev_res) {
  return RunContextUntil(event, GetBranchUuid(branch), ev_res);
}

void BranchEventRecorder::StartAwaitEvent() {
  int res = YOGI_BranchAwaitEvent(branch_, 0, &uuid_, json_str_.data(),
                                  json_str_.size(),
                                  &BranchEventRecorder::Callback, this);
  EXPECT_EQ(res, YOGI_OK);
}

void BranchEventRecorder::Callback(int res, int event, int ev_res,
                                   void* userarg) {
  if (res == YOGI_ERR_CANCELED) return;

  auto self = static_cast<BranchEventRecorder*>(userarg);
  self->events_.push_back({self->uuid_,
                           nlohmann::json::parse(self->json_str_.data()), event,
                           ev_res});

  self->StartAwaitEvent();
}

void SetupLogging(int verbosity) {
  YOGI_LogToConsole(YOGI_VB_TRACE, YOGI_ST_STDERR, YOGI_TRUE, nullptr, nullptr);
  YOGI_LoggerSetComponentsVerbosity("Yogi\\..*", verbosity, nullptr);
}

void* CreateContext() {
  void* context = nullptr;
  int res = YOGI_ContextCreate(&context);
  EXPECT_EQ(res, YOGI_OK);
  EXPECT_NE(context, nullptr);
  return context;
}

void RunContextInBackground(void* context) {
  int res = YOGI_ContextRunInBackground(context);
  EXPECT_EQ(res, YOGI_OK);
  res = YOGI_ContextWaitForRunning(context, 1000000000);
  EXPECT_EQ(res, YOGI_OK);
}

void* CreateBranch(void* context, const char* name, const char* net_name,
                   const char* password, const char* path) {
  void* branch = nullptr;
  int res = YOGI_BranchCreate(&branch, context, name, nullptr, net_name,
                              password, path, nullptr, kAdvPort,
                              kAdvInterval.count(), kConnTimeout.count());
  EXPECT_EQ(res, YOGI_OK);
  return branch;
}

boost::asio::ip::tcp::endpoint GetBranchTcpEndpoint(void* branch) {
  char json_str[1000] = {0};
  YOGI_BranchGetInfo(branch, nullptr, json_str, sizeof(json_str));

  auto json = nlohmann::json::parse(json_str);
  auto port = json["tcp_server_port"].get<unsigned short>();

  return {boost::asio::ip::address::from_string("::1"), port};
}

boost::uuids::uuid GetBranchUuid(void* branch) {
  boost::uuids::uuid uuid = {0};
  int res = YOGI_BranchGetInfo(branch, &uuid, nullptr, 0);
  EXPECT_EQ(res, YOGI_OK);
  EXPECT_NE(uuid, boost::uuids::uuid{});
  return uuid;
}

nlohmann::json GetBranchInfo(void* branch) {
  char str[10000];
  int res = YOGI_BranchGetInfo(branch, nullptr, str, sizeof(str));
  EXPECT_EQ(res, YOGI_OK);
  return nlohmann::json::parse(str);
}

void CheckJsonElementsAreEqual(const nlohmann::json& a, const nlohmann::json& b,
                               const std::string& key) {
  ASSERT_TRUE(a.count(key)) << "Key \"" << key << "\" does not exist in a";
  ASSERT_TRUE(b.count(key)) << "Key \"" << key << "\" does not exist in b";
  EXPECT_EQ(a[key].dump(), b[key].dump());
}