#include <gtest/gtest.h>
#include <logger.h>
#include <stdio.h>
#include "mocks/FakeTimers.h"

namespace
{

    class MockOutput : public logger::Output
    {
        private:
            std::stringstream result;
        public:
            std::string currentOutput();

            void print(const char*);
            void print(char);
            void print(int, int);

            void clean();
    };

    std::string MockOutput::currentOutput() {
        return this->result.str();
    }

    void MockOutput::print(const char* val) {
        result << val;
    }

    void MockOutput::print(const char val) {
        result << val;
    }

    void MockOutput::print(int val, int format) {
        result << val;
    }

    void MockOutput::clean() {
        result = std::stringstream();
    }

    TEST(logger, printf)
    {
        MockOutput out;
        // std::string result = formatString(&out, "Size of char: %s", "hello");
        // std::cout << "result: " << out.currentOutput() << "\n";
        // out.clean();

        logger::printf(&out, "Regular message");
        EXPECT_EQ("Regular message", out.currentOutput());
        out.clean();

        // formatString(&out, "Encoded %%");
        // EXPECT_EQ("Encoded %", out.currentOutput());
        // out.clean();

        logger::printf(&out, "Single string param: %s", "str1");
        EXPECT_EQ("Single string param: str1", out.currentOutput());
        out.clean();

        logger::printf(&out, "Multiple string params %s, %s", "str1", "str2");
        EXPECT_EQ("Multiple string params str1, str2", out.currentOutput());
        out.clean();

        logger::printf(&out, "Number params %i, %i", 10, 20);
        EXPECT_EQ("Number params 10, 20", out.currentOutput());
        out.clean();

        // logger::printf(&out, "String params %");
        // EXPECT_EQ("String params str2, str2", out.currentOutput());
        // out.clean();
    }

    TEST(logger, Logger)
    {
        const int rnd = std::rand();
        FakeTimers fakeTimers;
        fakeTimers.setMillis(rnd);
        MockOutput out;
        logger::Logger logger(&fakeTimers, &out);
        logger.log("Info message: %s", "some val");
        std::string expected = "[INFO " + std::to_string(rnd) + "] Info message: some val\n";
        EXPECT_EQ(expected, out.currentOutput());
    }
} // namespace
