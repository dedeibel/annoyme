#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include <string>
#include <cstring>

using namespace std;

#include "exceptions.h"

#include "config/Configuration.h"
#include "config/BasicConfiguration.h"
#include "config/AnnoymeConfiguration.h"

class AnnoymeConfigurationTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(AnnoymeConfigurationTest);
		CPPUNIT_TEST(testGetDynamicPrefixNoPath);
		CPPUNIT_TEST(testGetDynamicPrefixAbsolute);
		CPPUNIT_TEST(testGetDynamicPrefixRelative);
	CPPUNIT_TEST_SUITE_END();
private:
public:
	void testGetDynamicPrefixNoPath()
	{
		AnnoymeConfiguration* c = AnnoymeConfiguration::getInstance();
		CPPUNIT_ASSERT_EQUAL(string("/my/pwd"), c->getDynamicPrefix("/my/pwd", ""));
	}

	void testGetDynamicPrefixAbsolute()
	{
		AnnoymeConfiguration* c = AnnoymeConfiguration::getInstance();
		CPPUNIT_ASSERT_EQUAL(string("/my/binary"), c->getDynamicPrefix("/my/pwd", "/my/binary/bin/app"));
	}

	void testGetDynamicPrefixRelative()
	{
		AnnoymeConfiguration* c = AnnoymeConfiguration::getInstance();
		CPPUNIT_ASSERT_EQUAL(string("/my/pwd"), c->getDynamicPrefix("/my/pwd", "./bin/app"));
		CPPUNIT_ASSERT_EQUAL(string("/my/pwd"), c->getDynamicPrefix("/my/pwd/bin", "./app"));
		// TODO test more complex paths like ././bin or /bin/../nobin/path
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION(AnnoymeConfigurationTest);
