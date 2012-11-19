// Copyright (C) 2005-2010 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/pre.hxx>

#include "simulationConfig.h"

// FloatVector
// 

const FloatVector::X1Type& FloatVector::
x1 () const
{
  return this->x1_.get ();
}

FloatVector::X1Type& FloatVector::
x1 ()
{
  return this->x1_.get ();
}

void FloatVector::
x1 (const X1Type& x)
{
  this->x1_.set (x);
}

const FloatVector::X2Type& FloatVector::
x2 () const
{
  return this->x2_.get ();
}

FloatVector::X2Type& FloatVector::
x2 ()
{
  return this->x2_.get ();
}

void FloatVector::
x2 (const X2Type& x)
{
  this->x2_.set (x);
}

const FloatVector::X3Type& FloatVector::
x3 () const
{
  return this->x3_.get ();
}

FloatVector::X3Type& FloatVector::
x3 ()
{
  return this->x3_.get ();
}

void FloatVector::
x3 (const X3Type& x)
{
  this->x3_.set (x);
}


// IntVector
// 

const IntVector::X1Type& IntVector::
x1 () const
{
  return this->x1_.get ();
}

IntVector::X1Type& IntVector::
x1 ()
{
  return this->x1_.get ();
}

void IntVector::
x1 (const X1Type& x)
{
  this->x1_.set (x);
}

const IntVector::X2Type& IntVector::
x2 () const
{
  return this->x2_.get ();
}

IntVector::X2Type& IntVector::
x2 ()
{
  return this->x2_.get ();
}

void IntVector::
x2 (const X2Type& x)
{
  this->x2_.set (x);
}

const IntVector::X3Type& IntVector::
x3 () const
{
  return this->x3_.get ();
}

IntVector::X3Type& IntVector::
x3 ()
{
  return this->x3_.get ();
}

void IntVector::
x3 (const X3Type& x)
{
  this->x3_.set (x);
}


// Cuboid
// 

const Cuboid::BottomLeftType& Cuboid::
bottomLeft () const
{
  return this->bottomLeft_.get ();
}

Cuboid::BottomLeftType& Cuboid::
bottomLeft ()
{
  return this->bottomLeft_.get ();
}

void Cuboid::
bottomLeft (const BottomLeftType& x)
{
  this->bottomLeft_.set (x);
}

void Cuboid::
bottomLeft (::std::auto_ptr< BottomLeftType > x)
{
  this->bottomLeft_.set (x);
}

const Cuboid::NXType& Cuboid::
nX () const
{
  return this->nX_.get ();
}

Cuboid::NXType& Cuboid::
nX ()
{
  return this->nX_.get ();
}

void Cuboid::
nX (const NXType& x)
{
  this->nX_.set (x);
}

void Cuboid::
nX (::std::auto_ptr< NXType > x)
{
  this->nX_.set (x);
}

const Cuboid::StepWidthType& Cuboid::
stepWidth () const
{
  return this->stepWidth_.get ();
}

Cuboid::StepWidthType& Cuboid::
stepWidth ()
{
  return this->stepWidth_.get ();
}

void Cuboid::
stepWidth (const StepWidthType& x)
{
  this->stepWidth_.set (x);
}

const Cuboid::MassType& Cuboid::
mass () const
{
  return this->mass_.get ();
}

Cuboid::MassType& Cuboid::
mass ()
{
  return this->mass_.get ();
}

void Cuboid::
mass (const MassType& x)
{
  this->mass_.set (x);
}

const Cuboid::TypeType& Cuboid::
type () const
{
  return this->type_.get ();
}

Cuboid::TypeType& Cuboid::
type ()
{
  return this->type_.get ();
}

void Cuboid::
type (const TypeType& x)
{
  this->type_.set (x);
}

const Cuboid::InitialVelocityType& Cuboid::
initialVelocity () const
{
  return this->initialVelocity_.get ();
}

Cuboid::InitialVelocityType& Cuboid::
initialVelocity ()
{
  return this->initialVelocity_.get ();
}

void Cuboid::
initialVelocity (const InitialVelocityType& x)
{
  this->initialVelocity_.set (x);
}

void Cuboid::
initialVelocity (::std::auto_ptr< InitialVelocityType > x)
{
  this->initialVelocity_.set (x);
}

const Cuboid::BrownianMeanVelocityType& Cuboid::
brownianMeanVelocity () const
{
  return this->brownianMeanVelocity_.get ();
}

Cuboid::BrownianMeanVelocityType& Cuboid::
brownianMeanVelocity ()
{
  return this->brownianMeanVelocity_.get ();
}

void Cuboid::
brownianMeanVelocity (const BrownianMeanVelocityType& x)
{
  this->brownianMeanVelocity_.set (x);
}


// Generator
// 

const Generator::CuboidSequence& Generator::
cuboid () const
{
  return this->cuboid_;
}

Generator::CuboidSequence& Generator::
cuboid ()
{
  return this->cuboid_;
}

void Generator::
cuboid (const CuboidSequence& s)
{
  this->cuboid_ = s;
}


// SimulationConfig
// 

const SimulationConfig::DeltaTType& SimulationConfig::
deltaT () const
{
  return this->deltaT_.get ();
}

SimulationConfig::DeltaTType& SimulationConfig::
deltaT ()
{
  return this->deltaT_.get ();
}

void SimulationConfig::
deltaT (const DeltaTType& x)
{
  this->deltaT_.set (x);
}

const SimulationConfig::EndTimeType& SimulationConfig::
endTime () const
{
  return this->endTime_.get ();
}

SimulationConfig::EndTimeType& SimulationConfig::
endTime ()
{
  return this->endTime_.get ();
}

void SimulationConfig::
endTime (const EndTimeType& x)
{
  this->endTime_.set (x);
}

const SimulationConfig::ScenarioTypeType& SimulationConfig::
scenarioType () const
{
  return this->scenarioType_.get ();
}

SimulationConfig::ScenarioTypeType& SimulationConfig::
scenarioType ()
{
  return this->scenarioType_.get ();
}

void SimulationConfig::
scenarioType (const ScenarioTypeType& x)
{
  this->scenarioType_.set (x);
}

void SimulationConfig::
scenarioType (::std::auto_ptr< ScenarioTypeType > x)
{
  this->scenarioType_.set (x);
}

const SimulationConfig::EpsilonType& SimulationConfig::
epsilon () const
{
  return this->epsilon_.get ();
}

SimulationConfig::EpsilonType& SimulationConfig::
epsilon ()
{
  return this->epsilon_.get ();
}

void SimulationConfig::
epsilon (const EpsilonType& x)
{
  this->epsilon_.set (x);
}

const SimulationConfig::SigmaType& SimulationConfig::
sigma () const
{
  return this->sigma_.get ();
}

SimulationConfig::SigmaType& SimulationConfig::
sigma ()
{
  return this->sigma_.get ();
}

void SimulationConfig::
sigma (const SigmaType& x)
{
  this->sigma_.set (x);
}

const SimulationConfig::OutputFrequencyType& SimulationConfig::
outputFrequency () const
{
  return this->outputFrequency_.get ();
}

SimulationConfig::OutputFrequencyType& SimulationConfig::
outputFrequency ()
{
  return this->outputFrequency_.get ();
}

void SimulationConfig::
outputFrequency (const OutputFrequencyType& x)
{
  this->outputFrequency_.set (x);
}

const SimulationConfig::DisableOutputType& SimulationConfig::
disableOutput () const
{
  return this->disableOutput_.get ();
}

SimulationConfig::DisableOutputType& SimulationConfig::
disableOutput ()
{
  return this->disableOutput_.get ();
}

void SimulationConfig::
disableOutput (const DisableOutputType& x)
{
  this->disableOutput_.set (x);
}

const SimulationConfig::LoggerConfigFileType& SimulationConfig::
loggerConfigFile () const
{
  return this->loggerConfigFile_.get ();
}

SimulationConfig::LoggerConfigFileType& SimulationConfig::
loggerConfigFile ()
{
  return this->loggerConfigFile_.get ();
}

void SimulationConfig::
loggerConfigFile (const LoggerConfigFileType& x)
{
  this->loggerConfigFile_.set (x);
}

void SimulationConfig::
loggerConfigFile (::std::auto_ptr< LoggerConfigFileType > x)
{
  this->loggerConfigFile_.set (x);
}

const SimulationConfig::OutputFilePrefixType& SimulationConfig::
outputFilePrefix () const
{
  return this->outputFilePrefix_.get ();
}

SimulationConfig::OutputFilePrefixType& SimulationConfig::
outputFilePrefix ()
{
  return this->outputFilePrefix_.get ();
}

void SimulationConfig::
outputFilePrefix (const OutputFilePrefixType& x)
{
  this->outputFilePrefix_.set (x);
}

void SimulationConfig::
outputFilePrefix (::std::auto_ptr< OutputFilePrefixType > x)
{
  this->outputFilePrefix_.set (x);
}

const SimulationConfig::InputFileType& SimulationConfig::
inputFile () const
{
  return this->inputFile_.get ();
}

SimulationConfig::InputFileType& SimulationConfig::
inputFile ()
{
  return this->inputFile_.get ();
}

void SimulationConfig::
inputFile (const InputFileType& x)
{
  this->inputFile_.set (x);
}

void SimulationConfig::
inputFile (::std::auto_ptr< InputFileType > x)
{
  this->inputFile_.set (x);
}

const SimulationConfig::GeneratorType& SimulationConfig::
generator () const
{
  return this->generator_.get ();
}

SimulationConfig::GeneratorType& SimulationConfig::
generator ()
{
  return this->generator_.get ();
}

void SimulationConfig::
generator (const GeneratorType& x)
{
  this->generator_.set (x);
}

void SimulationConfig::
generator (::std::auto_ptr< GeneratorType > x)
{
  this->generator_.set (x);
}


#include <xsd/cxx/xml/dom/parsing-source.hxx>

// FloatVector
//

FloatVector::
FloatVector (const X1Type& x1,
             const X2Type& x2,
             const X3Type& x3)
: ::xml_schema::Type (),
  x1_ (x1, ::xml_schema::Flags (), this),
  x2_ (x2, ::xml_schema::Flags (), this),
  x3_ (x3, ::xml_schema::Flags (), this)
{
}

FloatVector::
FloatVector (const FloatVector& x,
             ::xml_schema::Flags f,
             ::xml_schema::Container* c)
: ::xml_schema::Type (x, f, c),
  x1_ (x.x1_, f, this),
  x2_ (x.x2_, f, this),
  x3_ (x.x3_, f, this)
{
}

FloatVector::
FloatVector (const ::xercesc::DOMElement& e,
             ::xml_schema::Flags f,
             ::xml_schema::Container* c)
: ::xml_schema::Type (e, f | ::xml_schema::Flags::base, c),
  x1_ (f, this),
  x2_ (f, this),
  x3_ (f, this)
{
  if ((f & ::xml_schema::Flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, false, true);
    this->parse (p, f);
  }
}

void FloatVector::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::Flags f)
{
  while (p.more_attributes ())
  {
    const ::xercesc::DOMAttr& i (p.next_attribute ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    if (n.name () == "x1" && n.namespace_ ().empty ())
    {
      this->x1_.set (X1Traits::create (i, f, this));
      continue;
    }

    if (n.name () == "x2" && n.namespace_ ().empty ())
    {
      this->x2_.set (X2Traits::create (i, f, this));
      continue;
    }

    if (n.name () == "x3" && n.namespace_ ().empty ())
    {
      this->x3_.set (X3Traits::create (i, f, this));
      continue;
    }
  }

  if (!x1_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "x1",
      "");
  }

  if (!x2_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "x2",
      "");
  }

  if (!x3_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "x3",
      "");
  }
}

FloatVector* FloatVector::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class FloatVector (*this, f, c);
}

FloatVector::
~FloatVector ()
{
}

// IntVector
//

IntVector::
IntVector (const X1Type& x1,
           const X2Type& x2,
           const X3Type& x3)
: ::xml_schema::Type (),
  x1_ (x1, ::xml_schema::Flags (), this),
  x2_ (x2, ::xml_schema::Flags (), this),
  x3_ (x3, ::xml_schema::Flags (), this)
{
}

IntVector::
IntVector (const IntVector& x,
           ::xml_schema::Flags f,
           ::xml_schema::Container* c)
: ::xml_schema::Type (x, f, c),
  x1_ (x.x1_, f, this),
  x2_ (x.x2_, f, this),
  x3_ (x.x3_, f, this)
{
}

IntVector::
IntVector (const ::xercesc::DOMElement& e,
           ::xml_schema::Flags f,
           ::xml_schema::Container* c)
: ::xml_schema::Type (e, f | ::xml_schema::Flags::base, c),
  x1_ (f, this),
  x2_ (f, this),
  x3_ (f, this)
{
  if ((f & ::xml_schema::Flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, false, true);
    this->parse (p, f);
  }
}

void IntVector::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::Flags f)
{
  while (p.more_attributes ())
  {
    const ::xercesc::DOMAttr& i (p.next_attribute ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    if (n.name () == "x1" && n.namespace_ ().empty ())
    {
      this->x1_.set (X1Traits::create (i, f, this));
      continue;
    }

    if (n.name () == "x2" && n.namespace_ ().empty ())
    {
      this->x2_.set (X2Traits::create (i, f, this));
      continue;
    }

    if (n.name () == "x3" && n.namespace_ ().empty ())
    {
      this->x3_.set (X3Traits::create (i, f, this));
      continue;
    }
  }

  if (!x1_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "x1",
      "");
  }

  if (!x2_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "x2",
      "");
  }

  if (!x3_.present ())
  {
    throw ::xsd::cxx::tree::expected_attribute< char > (
      "x3",
      "");
  }
}

IntVector* IntVector::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class IntVector (*this, f, c);
}

IntVector::
~IntVector ()
{
}

// Cuboid
//

Cuboid::
Cuboid (const BottomLeftType& bottomLeft,
        const NXType& nX,
        const StepWidthType& stepWidth,
        const MassType& mass,
        const TypeType& type,
        const InitialVelocityType& initialVelocity,
        const BrownianMeanVelocityType& brownianMeanVelocity)
: ::xml_schema::Type (),
  bottomLeft_ (bottomLeft, ::xml_schema::Flags (), this),
  nX_ (nX, ::xml_schema::Flags (), this),
  stepWidth_ (stepWidth, ::xml_schema::Flags (), this),
  mass_ (mass, ::xml_schema::Flags (), this),
  type_ (type, ::xml_schema::Flags (), this),
  initialVelocity_ (initialVelocity, ::xml_schema::Flags (), this),
  brownianMeanVelocity_ (brownianMeanVelocity, ::xml_schema::Flags (), this)
{
}

Cuboid::
Cuboid (::std::auto_ptr< BottomLeftType >& bottomLeft,
        ::std::auto_ptr< NXType >& nX,
        const StepWidthType& stepWidth,
        const MassType& mass,
        const TypeType& type,
        ::std::auto_ptr< InitialVelocityType >& initialVelocity,
        const BrownianMeanVelocityType& brownianMeanVelocity)
: ::xml_schema::Type (),
  bottomLeft_ (bottomLeft, ::xml_schema::Flags (), this),
  nX_ (nX, ::xml_schema::Flags (), this),
  stepWidth_ (stepWidth, ::xml_schema::Flags (), this),
  mass_ (mass, ::xml_schema::Flags (), this),
  type_ (type, ::xml_schema::Flags (), this),
  initialVelocity_ (initialVelocity, ::xml_schema::Flags (), this),
  brownianMeanVelocity_ (brownianMeanVelocity, ::xml_schema::Flags (), this)
{
}

Cuboid::
Cuboid (const Cuboid& x,
        ::xml_schema::Flags f,
        ::xml_schema::Container* c)
: ::xml_schema::Type (x, f, c),
  bottomLeft_ (x.bottomLeft_, f, this),
  nX_ (x.nX_, f, this),
  stepWidth_ (x.stepWidth_, f, this),
  mass_ (x.mass_, f, this),
  type_ (x.type_, f, this),
  initialVelocity_ (x.initialVelocity_, f, this),
  brownianMeanVelocity_ (x.brownianMeanVelocity_, f, this)
{
}

Cuboid::
Cuboid (const ::xercesc::DOMElement& e,
        ::xml_schema::Flags f,
        ::xml_schema::Container* c)
: ::xml_schema::Type (e, f | ::xml_schema::Flags::base, c),
  bottomLeft_ (f, this),
  nX_ (f, this),
  stepWidth_ (f, this),
  mass_ (f, this),
  type_ (f, this),
  initialVelocity_ (f, this),
  brownianMeanVelocity_ (f, this)
{
  if ((f & ::xml_schema::Flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void Cuboid::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::Flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // bottomLeft
    //
    if (n.name () == "bottomLeft" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< BottomLeftType > r (
        BottomLeftTraits::create (i, f, this));

      if (!bottomLeft_.present ())
      {
        this->bottomLeft_.set (r);
        continue;
      }
    }

    // nX
    //
    if (n.name () == "nX" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< NXType > r (
        NXTraits::create (i, f, this));

      if (!nX_.present ())
      {
        this->nX_.set (r);
        continue;
      }
    }

    // stepWidth
    //
    if (n.name () == "stepWidth" && n.namespace_ ().empty ())
    {
      if (!stepWidth_.present ())
      {
        this->stepWidth_.set (StepWidthTraits::create (i, f, this));
        continue;
      }
    }

    // mass
    //
    if (n.name () == "mass" && n.namespace_ ().empty ())
    {
      if (!mass_.present ())
      {
        this->mass_.set (MassTraits::create (i, f, this));
        continue;
      }
    }

    // type
    //
    if (n.name () == "type" && n.namespace_ ().empty ())
    {
      if (!type_.present ())
      {
        this->type_.set (TypeTraits::create (i, f, this));
        continue;
      }
    }

    // initialVelocity
    //
    if (n.name () == "initialVelocity" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< InitialVelocityType > r (
        InitialVelocityTraits::create (i, f, this));

      if (!initialVelocity_.present ())
      {
        this->initialVelocity_.set (r);
        continue;
      }
    }

    // brownianMeanVelocity
    //
    if (n.name () == "brownianMeanVelocity" && n.namespace_ ().empty ())
    {
      if (!brownianMeanVelocity_.present ())
      {
        this->brownianMeanVelocity_.set (BrownianMeanVelocityTraits::create (i, f, this));
        continue;
      }
    }

    break;
  }

  if (!bottomLeft_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "bottomLeft",
      "");
  }

  if (!nX_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "nX",
      "");
  }

  if (!stepWidth_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "stepWidth",
      "");
  }

  if (!mass_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "mass",
      "");
  }

  if (!type_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "type",
      "");
  }

  if (!initialVelocity_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "initialVelocity",
      "");
  }

  if (!brownianMeanVelocity_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "brownianMeanVelocity",
      "");
  }
}

Cuboid* Cuboid::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class Cuboid (*this, f, c);
}

Cuboid::
~Cuboid ()
{
}

// Generator
//

Generator::
Generator ()
: ::xml_schema::Type (),
  cuboid_ (::xml_schema::Flags (), this)
{
}

Generator::
Generator (const Generator& x,
           ::xml_schema::Flags f,
           ::xml_schema::Container* c)
: ::xml_schema::Type (x, f, c),
  cuboid_ (x.cuboid_, f, this)
{
}

Generator::
Generator (const ::xercesc::DOMElement& e,
           ::xml_schema::Flags f,
           ::xml_schema::Container* c)
: ::xml_schema::Type (e, f | ::xml_schema::Flags::base, c),
  cuboid_ (f, this)
{
  if ((f & ::xml_schema::Flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void Generator::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::Flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // cuboid
    //
    if (n.name () == "cuboid" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< CuboidType > r (
        CuboidTraits::create (i, f, this));

      this->cuboid_.push_back (r);
      continue;
    }

    break;
  }
}

Generator* Generator::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class Generator (*this, f, c);
}

Generator::
~Generator ()
{
}

// SimulationConfig
//

SimulationConfig::
SimulationConfig (const DeltaTType& deltaT,
                  const EndTimeType& endTime,
                  const ScenarioTypeType& scenarioType,
                  const EpsilonType& epsilon,
                  const SigmaType& sigma,
                  const OutputFrequencyType& outputFrequency,
                  const DisableOutputType& disableOutput,
                  const LoggerConfigFileType& loggerConfigFile,
                  const OutputFilePrefixType& outputFilePrefix,
                  const InputFileType& inputFile,
                  const GeneratorType& generator)
: ::xml_schema::Type (),
  deltaT_ (deltaT, ::xml_schema::Flags (), this),
  endTime_ (endTime, ::xml_schema::Flags (), this),
  scenarioType_ (scenarioType, ::xml_schema::Flags (), this),
  epsilon_ (epsilon, ::xml_schema::Flags (), this),
  sigma_ (sigma, ::xml_schema::Flags (), this),
  outputFrequency_ (outputFrequency, ::xml_schema::Flags (), this),
  disableOutput_ (disableOutput, ::xml_schema::Flags (), this),
  loggerConfigFile_ (loggerConfigFile, ::xml_schema::Flags (), this),
  outputFilePrefix_ (outputFilePrefix, ::xml_schema::Flags (), this),
  inputFile_ (inputFile, ::xml_schema::Flags (), this),
  generator_ (generator, ::xml_schema::Flags (), this)
{
}

SimulationConfig::
SimulationConfig (const DeltaTType& deltaT,
                  const EndTimeType& endTime,
                  const ScenarioTypeType& scenarioType,
                  const EpsilonType& epsilon,
                  const SigmaType& sigma,
                  const OutputFrequencyType& outputFrequency,
                  const DisableOutputType& disableOutput,
                  const LoggerConfigFileType& loggerConfigFile,
                  const OutputFilePrefixType& outputFilePrefix,
                  const InputFileType& inputFile,
                  ::std::auto_ptr< GeneratorType >& generator)
: ::xml_schema::Type (),
  deltaT_ (deltaT, ::xml_schema::Flags (), this),
  endTime_ (endTime, ::xml_schema::Flags (), this),
  scenarioType_ (scenarioType, ::xml_schema::Flags (), this),
  epsilon_ (epsilon, ::xml_schema::Flags (), this),
  sigma_ (sigma, ::xml_schema::Flags (), this),
  outputFrequency_ (outputFrequency, ::xml_schema::Flags (), this),
  disableOutput_ (disableOutput, ::xml_schema::Flags (), this),
  loggerConfigFile_ (loggerConfigFile, ::xml_schema::Flags (), this),
  outputFilePrefix_ (outputFilePrefix, ::xml_schema::Flags (), this),
  inputFile_ (inputFile, ::xml_schema::Flags (), this),
  generator_ (generator, ::xml_schema::Flags (), this)
{
}

SimulationConfig::
SimulationConfig (const SimulationConfig& x,
                  ::xml_schema::Flags f,
                  ::xml_schema::Container* c)
: ::xml_schema::Type (x, f, c),
  deltaT_ (x.deltaT_, f, this),
  endTime_ (x.endTime_, f, this),
  scenarioType_ (x.scenarioType_, f, this),
  epsilon_ (x.epsilon_, f, this),
  sigma_ (x.sigma_, f, this),
  outputFrequency_ (x.outputFrequency_, f, this),
  disableOutput_ (x.disableOutput_, f, this),
  loggerConfigFile_ (x.loggerConfigFile_, f, this),
  outputFilePrefix_ (x.outputFilePrefix_, f, this),
  inputFile_ (x.inputFile_, f, this),
  generator_ (x.generator_, f, this)
{
}

SimulationConfig::
SimulationConfig (const ::xercesc::DOMElement& e,
                  ::xml_schema::Flags f,
                  ::xml_schema::Container* c)
: ::xml_schema::Type (e, f | ::xml_schema::Flags::base, c),
  deltaT_ (f, this),
  endTime_ (f, this),
  scenarioType_ (f, this),
  epsilon_ (f, this),
  sigma_ (f, this),
  outputFrequency_ (f, this),
  disableOutput_ (f, this),
  loggerConfigFile_ (f, this),
  outputFilePrefix_ (f, this),
  inputFile_ (f, this),
  generator_ (f, this)
{
  if ((f & ::xml_schema::Flags::base) == 0)
  {
    ::xsd::cxx::xml::dom::parser< char > p (e, true, false);
    this->parse (p, f);
  }
}

void SimulationConfig::
parse (::xsd::cxx::xml::dom::parser< char >& p,
       ::xml_schema::Flags f)
{
  for (; p.more_elements (); p.next_element ())
  {
    const ::xercesc::DOMElement& i (p.cur_element ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (i));

    // deltaT
    //
    if (n.name () == "deltaT" && n.namespace_ ().empty ())
    {
      if (!deltaT_.present ())
      {
        this->deltaT_.set (DeltaTTraits::create (i, f, this));
        continue;
      }
    }

    // endTime
    //
    if (n.name () == "endTime" && n.namespace_ ().empty ())
    {
      if (!endTime_.present ())
      {
        this->endTime_.set (EndTimeTraits::create (i, f, this));
        continue;
      }
    }

    // scenarioType
    //
    if (n.name () == "scenarioType" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< ScenarioTypeType > r (
        ScenarioTypeTraits::create (i, f, this));

      if (!scenarioType_.present ())
      {
        this->scenarioType_.set (r);
        continue;
      }
    }

    // epsilon
    //
    if (n.name () == "epsilon" && n.namespace_ ().empty ())
    {
      if (!epsilon_.present ())
      {
        this->epsilon_.set (EpsilonTraits::create (i, f, this));
        continue;
      }
    }

    // sigma
    //
    if (n.name () == "sigma" && n.namespace_ ().empty ())
    {
      if (!sigma_.present ())
      {
        this->sigma_.set (SigmaTraits::create (i, f, this));
        continue;
      }
    }

    // outputFrequency
    //
    if (n.name () == "outputFrequency" && n.namespace_ ().empty ())
    {
      if (!outputFrequency_.present ())
      {
        this->outputFrequency_.set (OutputFrequencyTraits::create (i, f, this));
        continue;
      }
    }

    // disableOutput
    //
    if (n.name () == "disableOutput" && n.namespace_ ().empty ())
    {
      if (!disableOutput_.present ())
      {
        this->disableOutput_.set (DisableOutputTraits::create (i, f, this));
        continue;
      }
    }

    // loggerConfigFile
    //
    if (n.name () == "loggerConfigFile" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< LoggerConfigFileType > r (
        LoggerConfigFileTraits::create (i, f, this));

      if (!loggerConfigFile_.present ())
      {
        this->loggerConfigFile_.set (r);
        continue;
      }
    }

    // outputFilePrefix
    //
    if (n.name () == "outputFilePrefix" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< OutputFilePrefixType > r (
        OutputFilePrefixTraits::create (i, f, this));

      if (!outputFilePrefix_.present ())
      {
        this->outputFilePrefix_.set (r);
        continue;
      }
    }

    // inputFile
    //
    if (n.name () == "inputFile" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< InputFileType > r (
        InputFileTraits::create (i, f, this));

      if (!inputFile_.present ())
      {
        this->inputFile_.set (r);
        continue;
      }
    }

    // generator
    //
    if (n.name () == "generator" && n.namespace_ ().empty ())
    {
      ::std::auto_ptr< GeneratorType > r (
        GeneratorTraits::create (i, f, this));

      if (!generator_.present ())
      {
        this->generator_.set (r);
        continue;
      }
    }

    break;
  }

  if (!deltaT_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "deltaT",
      "");
  }

  if (!endTime_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "endTime",
      "");
  }

  if (!scenarioType_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "scenarioType",
      "");
  }

  if (!epsilon_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "epsilon",
      "");
  }

  if (!sigma_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "sigma",
      "");
  }

  if (!outputFrequency_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "outputFrequency",
      "");
  }

  if (!disableOutput_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "disableOutput",
      "");
  }

  if (!loggerConfigFile_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "loggerConfigFile",
      "");
  }

  if (!outputFilePrefix_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "outputFilePrefix",
      "");
  }

  if (!inputFile_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "inputFile",
      "");
  }

  if (!generator_.present ())
  {
    throw ::xsd::cxx::tree::expected_element< char > (
      "generator",
      "");
  }
}

SimulationConfig* SimulationConfig::
_clone (::xml_schema::Flags f,
        ::xml_schema::Container* c) const
{
  return new class SimulationConfig (*this, f, c);
}

SimulationConfig::
~SimulationConfig ()
{
}

#include <istream>
#include <xsd/cxx/xml/sax/std-input-source.hxx>
#include <xsd/cxx/tree/error-handler.hxx>

::std::auto_ptr< ::SimulationConfig >
simulationConfig (const ::std::string& u,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xsd::cxx::tree::error_handler< char > h;

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

  ::std::auto_ptr< ::SimulationConfig > r (
    ::simulationConfig (
      d, f | ::xml_schema::Flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (const ::std::string& u,
                  ::xml_schema::ErrorHandler& h,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::SimulationConfig > r (
    ::simulationConfig (
      d, f | ::xml_schema::Flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (const ::std::string& u,
                  ::xercesc::DOMErrorHandler& h,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      u, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::SimulationConfig > r (
    ::simulationConfig (
      d, f | ::xml_schema::Flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (::std::istream& is,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::simulationConfig (isrc, f, p);
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (::std::istream& is,
                  ::xml_schema::ErrorHandler& h,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::simulationConfig (isrc, h, f, p);
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (::std::istream& is,
                  ::xercesc::DOMErrorHandler& h,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is);
  return ::simulationConfig (isrc, h, f, p);
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (::std::istream& is,
                  const ::std::string& sid,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::simulationConfig (isrc, f, p);
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (::std::istream& is,
                  const ::std::string& sid,
                  ::xml_schema::ErrorHandler& h,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::auto_initializer i (
    (f & ::xml_schema::Flags::dont_initialize) == 0,
    (f & ::xml_schema::Flags::keep_dom) == 0);

  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::simulationConfig (isrc, h, f, p);
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (::std::istream& is,
                  const ::std::string& sid,
                  ::xercesc::DOMErrorHandler& h,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
  return ::simulationConfig (isrc, h, f, p);
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (::xercesc::InputSource& i,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xsd::cxx::tree::error_handler< char > h;

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

  ::std::auto_ptr< ::SimulationConfig > r (
    ::simulationConfig (
      d, f | ::xml_schema::Flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (::xercesc::InputSource& i,
                  ::xml_schema::ErrorHandler& h,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::SimulationConfig > r (
    ::simulationConfig (
      d, f | ::xml_schema::Flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (::xercesc::InputSource& i,
                  ::xercesc::DOMErrorHandler& h,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
    ::xsd::cxx::xml::dom::parse< char > (
      i, h, p, f));

  if (!d.get ())
    throw ::xsd::cxx::tree::parsing< char > ();

  ::std::auto_ptr< ::SimulationConfig > r (
    ::simulationConfig (
      d, f | ::xml_schema::Flags::own_dom, p));

  return r;
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (const ::xercesc::DOMDocument& d,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties& p)
{
  if (f & ::xml_schema::Flags::keep_dom)
  {
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > c (
      static_cast< ::xercesc::DOMDocument* > (d.cloneNode (true)));

    ::std::auto_ptr< ::SimulationConfig > r (
      ::simulationConfig (
        c, f | ::xml_schema::Flags::own_dom, p));

    return r;
  }

  const ::xercesc::DOMElement& e (*d.getDocumentElement ());
  const ::xsd::cxx::xml::qualified_name< char > n (
    ::xsd::cxx::xml::dom::name< char > (e));

  if (n.name () == "simulationConfig" &&
      n.namespace_ () == "")
  {
    ::std::auto_ptr< ::SimulationConfig > r (
      ::xsd::cxx::tree::traits< ::SimulationConfig, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "simulationConfig",
    "");
}

::std::auto_ptr< ::SimulationConfig >
simulationConfig (::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument >& d,
                  ::xml_schema::Flags f,
                  const ::xml_schema::Properties&)
{
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > c (
    ((f & ::xml_schema::Flags::keep_dom) &&
     !(f & ::xml_schema::Flags::own_dom))
    ? static_cast< ::xercesc::DOMDocument* > (d->cloneNode (true))
    : 0);

  ::xercesc::DOMDocument& doc (c.get () ? *c : *d);
  const ::xercesc::DOMElement& e (*doc.getDocumentElement ());

  const ::xsd::cxx::xml::qualified_name< char > n (
    ::xsd::cxx::xml::dom::name< char > (e));

  if (f & ::xml_schema::Flags::keep_dom)
    doc.setUserData (::xml_schema::dom::tree_node_key,
                     (c.get () ? &c : &d),
                     0);

  if (n.name () == "simulationConfig" &&
      n.namespace_ () == "")
  {
    ::std::auto_ptr< ::SimulationConfig > r (
      ::xsd::cxx::tree::traits< ::SimulationConfig, char >::create (
        e, f, 0));
    return r;
  }

  throw ::xsd::cxx::tree::unexpected_element < char > (
    n.name (),
    n.namespace_ (),
    "simulationConfig",
    "");
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

