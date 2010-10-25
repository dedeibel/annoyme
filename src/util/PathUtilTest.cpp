#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

#include <string>
#include <cstring>

using namespace std;

#include "exceptions.h"

#include "util/PathUtil.h"

extern "C"
{
#include <sys/stat.h>
#include <sys/types.h>
}
#include <cstdlib>

class PathUtilTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(PathUtilTest);
		CPPUNIT_TEST(testGetDynamicPrefixNoPath);
		CPPUNIT_TEST(testGetDynamicPrefixAbsolute);
		CPPUNIT_TEST(testGetDynamicPrefixRelative);
	CPPUNIT_TEST_SUITE_END();
private:
	char* m_dir_template;
	std::string m_basepath;

public:
	void setUp()
	{
		// Create a temp dir
		m_dir_template = strdup("/tmp/annoyme_pathutiltest_XXXXXX");
		CPPUNIT_ASSERT(mkdtemp(m_dir_template) != 0);
		m_basepath = string(m_dir_template);

		// Create a bin directory
		string binpath = m_basepath + "/bin";
		CPPUNIT_ASSERT(mkdir(binpath.c_str(), S_IRUSR | S_IWUSR | S_IXUSR) == 0);

		string binfile = binpath + "/app";
		// Create a binary file
		FILE* binfilefh = fopen(binfile.c_str(), "w");
		CPPUNIT_ASSERT(binfilefh != 0);
		CPPUNIT_ASSERT(fclose(binfilefh) == 0);
	}

	void tearDown()
	{
		// TODO write file util method
		if (m_basepath.find("/tmp/") != std::string::npos) {
			system((string("/bin/rm -r \"") + m_basepath + "\"").c_str());
		}
		free(m_dir_template);
	}

	void testGetDynamicPrefixNoPath()
	{
		PathUtil util;
		CPPUNIT_ASSERT_EQUAL(m_basepath, util.getDynamicPrefix(m_basepath, ""));
	}

	void testGetDynamicPrefixAbsolute()
	{
		PathUtil util;
		CPPUNIT_ASSERT_EQUAL(m_basepath, util.getDynamicPrefix(m_basepath, m_basepath + "/bin/app"));
	}

	void testGetDynamicPrefixRelative()
	{
		PathUtil util;
		CPPUNIT_ASSERT_EQUAL(m_basepath, util.getDynamicPrefix(m_basepath, "./bin/app"));
		CPPUNIT_ASSERT_EQUAL(m_basepath, util.getDynamicPrefix(m_basepath + "/bin", "./app"));
	}

	void testGetDynamicPrefixNormalize()
	{
		PathUtil util;
		CPPUNIT_ASSERT_EQUAL(m_basepath, util.getDynamicPrefix(m_basepath + "/./bin/../", "./bin/app"));
		CPPUNIT_ASSERT_EQUAL(m_basepath, util.getDynamicPrefix(m_basepath + "//./", m_basepath + "/../../" + m_basepath + "/bin/app"));
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION(PathUtilTest);
