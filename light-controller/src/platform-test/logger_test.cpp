#include <ArduinoUnit.h>
#include <logger.h>

class MockOutput : public logger::Output
{
    private:
        String result;
    public:
        String currentOutput();

        void print(const char*);
        void print(char);
        void print(int, int);

        void clean();
};

String MockOutput::currentOutput() {
    return this->result;
}

void MockOutput::print(const char* val) {
    result += val;
}

void MockOutput::print(const char val) {
    result += val;
}

void MockOutput::print(int val, int format) {
    result += val;
}

void MockOutput::clean() {
    result = "";
}

test(logger_printf)
{
    MockOutput out;
    logger::printf(&out, "String with params: %s, %s, %d, %i", "str1", "str2", 10, 20);
    assertEqual("String with params: str1, str2, 10, 20", out.currentOutput());
    out.clean();
}

test(logger_printf_serial)
{
    logger::PrintOutput out(&Serial);
    logger::printf(&out, "Actual output - String with params: %s, %s, %d, %i, %b\n", "str1", "str2", 10, 20, 15);
    Serial.println("Expected output - String with params: str1, str2, 10, 20, 1111");
    Serial.println("Please compare manually");
}

test(logger_log)
{
    MockOutput out;
    Timers timers;
    logger::Logger logger(&timers, &out);
    logger.log("Sample log: %s, %s", "str1", "str2");
    assertEqual("[INFO 0] Sample log: str1, str2\n", out.currentOutput());
}

test(logger_macros)
{
    logger_log("String with params %s, %s", "param1", "param2");
    Serial.println("Expected output - String with params: param1, param2");
    Serial.println("Please compare manually");
}