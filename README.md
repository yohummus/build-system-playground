# Windows x86, x64 and VS solution
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x86-windows-static -DCMAKE_BUILD_TYPE=Debug -G "NMake Makefiles"
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_BUILD_TYPE=Debug -G "NMake Makefiles"
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x86-windows-static -DCMAKE_BUILD_TYPE=Debug

# Linux
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake

# Run Python tests manually
cd "/c/Documents and Settings/commu_000/Documents/Projects/build-system-playground/yogi-python"
YOGI_CORE_LIBRARY=../build-x64/yogi-core/yogi-core-x64.dll python -m unittest

# Run .NET tests manually
cd ../yogi-dotnet
YOGI_CORE_LIBRARY="/c/Documents and Settings/commu_000/Documents/Projects/build-system-playground/build-x64/yogi-core/yogi-core-x64.dll" dotnet test



TODO:
--------------------------------------------------
* Checkout asio timer problem (see code below)
* Check path validity when creating branches and terminals
* Whether or not a branch is actively receiving broadcasts must be conveyed when connecting to other branches in one of the initial messages to avoid missing broadcasts
* Add connection statistics (msg/sec, bytes/sec, etc.)
* Remote bytes_written/bytes_read from Transport::SendAll/ReceiveAll?

Outgoing data queue design:
* Needs size limit, otherwise it could potentially grow to GBs if the user does something dumb
* Could block send() if queue is full. That's fine if context.run() is running in a background thread.
* Give send() function an option to block or fail when queue is full. If context.run() is not running in background thread user might be screwed if blocking is set.

#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
using namespace std::chrono_literals;

TEST(Boost, AsioBug) {
  boost::asio::io_context ioc;
  boost::asio::steady_timer timer(ioc);
  timer.expires_after(0ns);
  timer.async_wait([](auto& ec) { std::cout << ec.message() << std::endl; });
  ioc.run_one();
}



Car
+--- Propulsion
|    +--- Engine [Engine Interface Provider]
|         +--- RPM
|         +--- Start
+--- Doors Locked
+--- Windows open
+--- User Interface
     +--- Dashboard [Engine Interface Consumer]
          +--- RPM
          +--- Start



Interface Description:
{
  "name": "Engine",
  "description": "Interface for controlling and monitoring the engine",
  "terminals": [
    {
      "path": "RPM",
      "type": "Publish",
      "description": "Rounds Per Minute",
      "data": [
        {
          "type": "float",
          "description": "Rounds per minute with 0.1 RPM accuracy",
          "min": 0,
          "max": 5000
        }
      ]
    },
    {
      "path": "Start",
      "type": "Respond",
      "description": "Starts the engine",
      "request": [],
      "response": [
        {
          "type": "bool",
          "description": "True if successful, false otherwise"
        }
      ]
    }
  ]
}


Terminal types:
* Constant - Constant
* Publisher - Subscriber
* Requester - Responder / Surveyor - Surveyee
* Pair - Pair
* Semaphore - Lock

* File - File
* Video - Video
* Audio - Audio

=> Should a subscriber be able to connect to a constant?
   This makes sense because if the receiver expects something that is not const
   then it doesn't mind if it is in fact const.

=> Should a publisher or subscriber be able to connect to a pair (one way comms)?
   Tricky because the semantics of a pair are "only connect with one other".

Every terminal is either tagged as provider or consumer. A process offering
an interface uses only provider terminals while processes using an interface
uses only consumer terminals. While provider terminals have paths (names)
describing where in their owning branch they appear, consumer terminals have
paths to the remote branch that they should be connected to. Consumer terminals
only ever connect to provider terminals; it's never the other way around. A
consumer terminal can be connected to at most a single provider terminal while
provider terminals can be connected to an arbitrary number of consumer terminals
if the terminal type supports it (e.g. Pair terminals only support a single
connection).
