using System;
using Xunit;

namespace test
{
    public class TimestampTest : TestCase
    {
        // [Fact]
        // public void FormatTime()
        // {
        //     var t = new DateTime(12343567891234567, DateTimeKind.Utc).AddYears(1969);

        //     var s = Yogi.FormatTime(t);
        //     Assert.IsType<string>(s);
        //     Assert.Equal("2009-02-11T12:53:09.123Z", s);

        //     s = Yogi.FormatTime(t, "%Y%m%d%H%M%S%3%6%9");
        //     Assert.Equal("20090211125309123456700", s);
        // }

        // [Fact]
        // public void ParseTime()
        // {
        //     var t = Yogi.ParseTime("2009-02-11T12:53:09.123Z");

        //     Assert.IsType<DateTime>(t);
        //     Assert.Equal(DateTimeKind.Utc, t.Kind);
        //     Assert.Equal(12343567891230000, t.AddYears(-1969).Ticks);

        //     t = Yogi.ParseTime("20090211125309123456789", "%Y%m%d%H%M%S%3%6%9");
        //     Assert.Equal(12343567891234567, t.AddYears(-1969).Ticks);
        // }

        [Fact]
        public void FromDurationSinceEpoch()
        {
            Assert.Equal(Yogi.Duration.FromDays(123), Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromDays(123)).DurationSinceEpoch);
            Assert.Throws<ArithmeticException>(() =>
            {
                var _ = Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.Infinity);
            });
            Assert.Throws<ArithmeticException>(() =>
            {
                var _ = Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(-1));
            });
        }

        [Fact]
        public void Now()
        {
            Assert.True(Yogi.Timestamp.Now.DurationSinceEpoch.TotalSeconds > 1e6);
        }

        [Fact]
        public void Parse()
        {
            var t = Yogi.Timestamp.Parse("2009-02-11T12:53:09.123Z");
            Assert.Equal(1234356789123000000L, t.DurationSinceEpoch.NanosecondsCount);

            t = Yogi.Timestamp.Parse("20090211125309123456789", "%Y%m%d%H%M%S%3%6%9");
            Assert.Equal(1234356789123456789L, t.DurationSinceEpoch.NanosecondsCount);
        }

        [Fact]
        public void Constructor()
        {
            Assert.Equal(0, new Yogi.Timestamp().DurationSinceEpoch.NanosecondsCount);
        }

        [Fact]
        public void DurationSinceEpoch()
        {
            Assert.Equal(Yogi.Duration.FromDays(123), Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromDays(123)).DurationSinceEpoch);
        }

        [Fact]
        public void Nanoseconds()
        {
            Assert.Equal(789, Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(123456789L)).Nanoseconds);
        }

        [Fact]
        public void Microseconds()
        {
            Assert.Equal(456, Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(123456789L)).Microseconds);
        }

        [Fact]
        public void Milliseconds()
        {
            Assert.Equal(123, Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(123456789L)).Milliseconds);
        }

        [Fact]
        public void Format()
        {
            var t = Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(1234356789123456789L));

            var s = t.Format();
            Assert.Equal("2009-02-11T12:53:09.123Z", s);

            s = t.Format("%Y%m%d%H%M%S%3%6%9");
            Assert.Equal("20090211125309123456789", s);
        }

        [Fact]
        public void ToStringMethod()
        {
            var t = Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(1234356789123456789L));
            Assert.Equal("2009-02-11T12:53:09.123Z", t.ToString());
        }

        [Fact]
        public void Operators()
        {
            var t = new Yogi.Timestamp();
            t += Yogi.Duration.FromNanoseconds(8);
            Assert.Equal(8, t.DurationSinceEpoch.NanosecondsCount);
            Assert.Throws<ArithmeticException>(() =>
            {
                t += Yogi.Duration.FromMilliseconds(-1);
            });
            t -= Yogi.Duration.FromNanoseconds(1);
            Assert.Equal(7, t.DurationSinceEpoch.NanosecondsCount);
            Assert.Throws<ArithmeticException>(() =>
            {
                t -= Yogi.Duration.FromMilliseconds(1);
            });

            Assert.Equal(12, (t + Yogi.Duration.FromNanoseconds(5)
                ).DurationSinceEpoch.NanosecondsCount);
            Assert.Throws<ArithmeticException>(() =>
            {
                var _ = t + Yogi.Duration.FromMilliseconds(-1);
            });
            Assert.Equal(5, (t - Yogi.Duration.FromNanoseconds(2)
                ).DurationSinceEpoch.NanosecondsCount);
            Assert.Equal(4, (t - Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(3))).NanosecondsCount);
            Assert.Throws<ArithmeticException>(() =>
            {
                var _ = t - Yogi.Duration.FromMilliseconds(1);
            });

            Assert.True(t == Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(7)));
            Assert.False(t == Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(8)));
            Assert.False(t != Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(7)));
            Assert.True(t != Yogi.Timestamp.FromDurationSinceEpoch(
                Yogi.Duration.FromNanoseconds(8)));

            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))
                < Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                < Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2)));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                < Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));

            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))
                > Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));
            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                > Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2)));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                > Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));

            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))
                <= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                <= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2)));
            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                <= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));

            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))
                >= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));
            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                >= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2)));
            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3))
                >= Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(3)));
        }

        [Fact]
        public void EqualsMethod()
        {
            Assert.True(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .Equals(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .Equals(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))));
            Assert.False(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .Equals(""));
        }

        [Fact]
        public void GetHashCodeMethod()
        {
            Assert.NotEqual(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1)),
                Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2)));
        }

        [Fact]
        public void CompareTo()
        {
            Assert.Equal(-1, Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .CompareTo(""));
            Assert.Equal(-1, Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .CompareTo(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))));
            Assert.Equal(0, Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))
                .CompareTo(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))));
            Assert.Equal(+1, Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(2))
                .CompareTo(Yogi.Timestamp.FromDurationSinceEpoch(Yogi.Duration.FromDays(1))));
        }
    }
}
