#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class SampleTest: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(SampleTest);
	CPPUNIT_TEST(testEmpty);
	CPPUNIT_TEST_SUITE_END();
public:
	void testEmpty()
	{
		int i = 5;
		CPPUNIT_ASSERT(i == 5);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(SampleTest);
