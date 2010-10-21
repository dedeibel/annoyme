#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main(int argc, char* argv[])
{
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry& registry =
			CppUnit::TestFactoryRegistry::getRegistry();

	// run all tests if none specified on command line
	CppUnit::Test* test_to_run = registry.makeTest();
	if (argc > 1) {
		test_to_run = test_to_run->findTest(argv[1]);
	}

	runner.addTest(test_to_run);
	bool failed = runner.run("", false);
	return !failed;
}
