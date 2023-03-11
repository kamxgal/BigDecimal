#pragma once

#include <iostream>
#include <string>
#include <vector>


#define ASSERT_EQ(a, b) if ((a) != (b)) { std::cout << "Line " << __LINE__ << ": ASSERT_EQ failed: " << a << " != " << b << std::endl; return false; }
#define ASSERT_NEQ(a, b) if ((a) == (b)) { std::cout << "Line " << __LINE__ << ": ASSERT_NEQ failed: " << a << " == " << b << std::endl; return false; }

struct TestSuite
{
    explicit TestSuite(std::string name) : name(std::move(name))
    {}

    void print_failed() {
        for (const auto& name : failed_tests) {
            std::cerr << "[FAILED] " << name << std::endl;
        }
    }

    const std::string& get_name() const {
        return name;
    }

    void add_failed_test(std::string testname) {
        failed_tests.emplace_back(std::move(testname));
    }

private:
    std::string name;
    std::vector<std::string> failed_tests;
};

#define EXECUTE_TEST(testSuite, testName) \
    std::cout << "[BEGIN]\t" << testSuite.get_name() << "." << #testName << std::endl; \
    if (testName()) { \
        std::cout << "[SUCCESS]\t" << testSuite.get_name() << "." << #testName << std::endl; \
    } else { \
        std::cout << "[FAILED]\t" << testSuite.get_name() << "." << #testName << std::endl; \
        testSuite.add_failed_test(std::string(#testName)); \
    }
