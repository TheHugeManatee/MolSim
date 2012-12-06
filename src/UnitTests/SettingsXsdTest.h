#include "utils/Settings.h"

#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

class SettingsXsdTest : public CppUnit::TestFixture{
public:
	static std::string xmlFileContents;

	void setUp() {
		std::ofstream t("temp.xml");

		t.write(xmlFileContents.c_str(), xmlFileContents.size());

		t.close();
	}


	void tearDown() {
		remove("temp.xml");
	}


	void testXml() {
		Settings::configFile = "temp.xml";
		char* argsv[] = {"MolSim"};
		Settings::initSettings(1, argsv);

		CPPUNIT_ASSERT(Settings::deltaT == 1.0);
		CPPUNIT_ASSERT(Settings::endTime == 10.0);
		CPPUNIT_ASSERT(Settings::scenarioType == ScenarioType::Lennard_Jones);
		CPPUNIT_ASSERT(Settings::domainSize[0] == Settings::domainSize[1]
		            && Settings::domainSize[1] == Settings::domainSize[2]
		            && Settings::domainSize[2] == 10.0);
		CPPUNIT_ASSERT(Settings::rCutoff == 1.0);
		CPPUNIT_ASSERT(Settings::containerType == ContainerType::CelllistContainer);
		CPPUNIT_ASSERT(Settings::epsilon == 3.0);
		CPPUNIT_ASSERT(Settings::sigma == 0.5);
		CPPUNIT_ASSERT(Settings::outputFrequency == 1000);
		CPPUNIT_ASSERT(!Settings::loggerConfigFile.compare("Logger.properties"));
		CPPUNIT_ASSERT(!Settings::outputFilePrefix.compare("testprefix"));
		CPPUNIT_ASSERT(!Settings::inputFile.compare("testfile"));
		CPPUNIT_ASSERT(Settings::outputFileType == OutputFileType::xyz);
		CPPUNIT_ASSERT(Settings::boundaryCondition[0] == BoundaryConditionType::Outflow);
		CPPUNIT_ASSERT(Settings::boundaryCondition[1] == BoundaryConditionType::Reflect);
		CPPUNIT_ASSERT(Settings::boundaryCondition[2] == BoundaryConditionType::Outflow);
		CPPUNIT_ASSERT(Settings::boundaryCondition[3] == BoundaryConditionType::Reflect);
		CPPUNIT_ASSERT(Settings::boundaryCondition[4] == BoundaryConditionType::Periodic);
		CPPUNIT_ASSERT(Settings::boundaryCondition[5] == BoundaryConditionType::Reflect);

	}

	/**
	 * create a test suite containing all tests for the ParticleContainer
	 */
	static CppUnit::Test *suite()
	{
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "SettingsXsdTest" );
		suiteOfTests->addTest( new CppUnit::TestCaller<SettingsXsdTest>(
									   "xml file test",
									   &SettingsXsdTest::testXml ) );
		return suiteOfTests;
	}

};


std::string SettingsXsdTest::xmlFileContents =
		"<?xml version=\"1.0\"?>                                                    \r\n"
		"                                                                           \r\n"
		"<simulationConfig xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"  \r\n"
		"       xsi:noNamespaceSchemaLocation=\"simulationConfig.xsd\">             \r\n"
		"                                                                           \r\n"
		"  <deltaT>1.0</deltaT>                                                     \r\n"
		"  <endTime>10</endTime>                                                    \r\n"
		"  <scenarioType>Lennard-Jones</scenarioType>                               \r\n"
		"  <domainSize x0=\"10.0\" x1=\"10.0\" x2=\"10.0\" />                       \r\n"
		"  <cutoffRadius>1.0</cutoffRadius>                                         \r\n"
		"                                                                           \r\n"
		"  <boundaryHandling                                                        \r\n"
		"  	top=\"Reflect\" bottom=\"Outflow\"                                      \r\n"
		"  	left=\"Outflow\" right=\"Reflect\"                                      \r\n"
		"  	front=\"Reflect\" back=\"Periodic\"                                     \r\n"
		"  />                                                                       \r\n"
		"  <containerType>CelllistContainer</containerType>                         \r\n"
		"  <epsilon>3</epsilon>                                                     \r\n"
		"  <sigma>0.5</sigma>                                                       \r\n"
		"  <outputFrequency>1000</outputFrequency>                                  \r\n"
		"  <disableOutput>1</disableOutput>                                         \r\n"
		"  <loggerConfigFile>Logger.properties</loggerConfigFile>                   \r\n"
		"  <outputFilePrefix>testprefix</outputFilePrefix>                          \r\n"
		"  <outputFileType>xyz</outputFileType>                                     \r\n"
		"  <inputFile>testfile</inputFile>                                          \r\n"
		"                                                                           \r\n"
		"  <generator>                                                              \r\n"
		"   <!-- <cuboid>                                                           \r\n"
		"      <bottomLeft x0=\"0.0\" x1=\"0.0\" x2=\"0.0\" />                      \r\n"
		"      <nX x0=\"8\" x1=\"8\" x2=\"8\" />                                    \r\n"
		"      <stepWidth>1.1225</stepWidth>                                        \r\n"
		"      <mass>1</mass>                                                       \r\n"
		"      <type>1</type>                                                       \r\n"
		"      <initialVelocity x1=\"0\" x2=\"10\" x3=\"0\"/>                       \r\n"
		"      <brownianMeanVelocity>0.1</brownianMeanVelocity>                     \r\n"
		"    </cuboid>                                                              \r\n"
		"    <cuboid>                                                               \r\n"
		"      <bottomLeft x0=\"5.0\" x1=\"5.0\" x2=\"5.0\" />                      \r\n"
		"      <nX x0=\"8\" x1=\"8\" x2=\"8\" />                                    \r\n"
		"      <stepWidth>1.1225</stepWidth>                                        \r\n"
		"      <mass>1</mass>                                                       \r\n"
		"      <type>1</type>                                                       \r\n"
		"      <initialVelocity x0=\"0\" x1=\"-10\" x2=\"0\"/>                      \r\n"
		"      <brownianMeanVelocity>0.1</brownianMeanVelocity>                     \r\n"
		"    </cuboid>-->                                                           \r\n"
		"                                                                           \r\n"
		"  </generator>                                                             \r\n"
		"                                                                           \r\n"
		"                                                                           \r\n"
		"</simulationConfig>";
