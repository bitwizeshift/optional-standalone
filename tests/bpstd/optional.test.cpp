/**
 * \file string_view.test.cpp
 *
 * \brief Unit tests for #bpstd::optional
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */

#include <bpstd/optional.hpp>

#include "../catch.hpp"

class CtorTest
{
public:
  explicit CtorTest( bool& is_called )
  {
    is_called = true;
  }
};

class CopyCtorTest
{
public:
  explicit CopyCtorTest( bool& is_called )
    : m_is_called(is_called)
  {

  }
  explicit CopyCtorTest( const CopyCtorTest& other )
    : m_is_called(other.m_is_called)
  {
    m_is_called = true;
  }
private:
  bool& m_is_called;
};

class MoveCtorTest
{
public:
  explicit MoveCtorTest( bool& is_called )
    : m_is_called(is_called)
  {

  }
  explicit MoveCtorTest( MoveCtorTest&& other )
    : m_is_called(other.m_is_called)
  {
    m_is_called = true;
  }
private:
  bool& m_is_called;
};

class CopyAssignTest
{
public:
  explicit CopyAssignTest( bool& is_called )
    : m_is_called(is_called)
  {

  }

  CopyAssignTest& operator = (const CopyAssignTest&)
  {
    m_is_called = true;
    return (*this);
  }

private:
  bool& m_is_called;
};

class MoveAssignTest
{
public:
  explicit MoveAssignTest( bool& is_called )
    : m_is_called(is_called)
  {

  }

  MoveAssignTest& operator = (MoveAssignTest&&)
  {
    m_is_called = true;
    return (*this);
  }

private:
  bool& m_is_called;
};

class DtorTest
{
public:
  explicit DtorTest( bool& is_called )
    : m_is_called(is_called)
  {

  }

  ~DtorTest()
  {
    m_is_called = true;
  }
private:
  bool& m_is_called;
};

//----------------------------------------------------------------------------
// Constructors / Destructor
//----------------------------------------------------------------------------

TEST_CASE("optional::optional()","[ctor]")
{
  auto optional = bpstd::optional<int>();

  SECTION("Has no value")
  {
    REQUIRE_FALSE( static_cast<bool>(optional) );
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::optional( nullopt_t )","[ctor]")
{
  auto optional = bpstd::optional<int>( bpstd::nullopt );

  SECTION("Has no value")
  {
    REQUIRE_FALSE( static_cast<bool>(optional) );
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::optional( const optional& )","[ctor]")
{
  auto is_called = false;

  SECTION("Copying a null optional")
  {
    auto original  = bpstd::optional<int>();
    auto optional  = original;

    SECTION("Has no value")
    {
      REQUIRE_FALSE( static_cast<bool>(optional) );
    }
  }

  SECTION("Copying a non-null optional")
  {
    auto value = 42;
    auto original  = bpstd::optional<int>( value );
    auto optional  = original;

    SECTION("Has a value")
    {
      REQUIRE( static_cast<bool>(optional) );
    }

    SECTION("Value is the same as original")
    {
      REQUIRE( optional.value() == value );
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::optional( optional&& )","[ctor]")
{
  SECTION("Copying a null optional")
  {
    auto original  = bpstd::optional<int>();
    auto optional  = std::move(original);

    SECTION("Has no value")
    {
      REQUIRE_FALSE( static_cast<bool>(optional) );
    }
  }

  SECTION("Copying a non-null optional")
  {
    auto value = 42;
    auto original  = bpstd::optional<int>( value );
    auto optional  = std::move(original);

    SECTION("Has a value")
    {
      REQUIRE( static_cast<bool>(optional) );
    }

    SECTION("Value is the same as original")
    {
      REQUIRE( optional.value() == value );
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::optional( const value_type& )","[ctor]")
{
  auto value    = 42;
  auto optional = bpstd::optional<int>(value);

  SECTION("Has a value")
  {
    REQUIRE( static_cast<bool>(optional) );
  }

  SECTION("Value is the same as original")
  {
    REQUIRE( optional.value() == value );
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::optional( value_type&& )","[ctor]")
{
  auto value    = 42;
  auto optional = bpstd::optional<int>( std::move(value) );

  SECTION("Has a value")
  {
    REQUIRE( static_cast<bool>(optional) );
  }

  SECTION("Value is the same as original")
  {
    REQUIRE( optional.value() == value );
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::~optional()","[dtor]")
{
  bool is_called = false;
  auto dtor = DtorTest(is_called);
  {
    auto optional = bpstd::optional<DtorTest>(dtor);
  }

  SECTION("Destructor gets called")
  {
    REQUIRE( is_called );
  }
}

//----------------------------------------------------------------------------
// Assignment
//----------------------------------------------------------------------------

TEST_CASE("optional::operator=( nullopt_t )","[assignment]")
{
  SECTION("Assigning over non-null value")
  {
    auto is_called = false;
    auto dtor = DtorTest(is_called);
    auto optional = bpstd::optional<DtorTest>(dtor);
    optional = bpstd::nullopt;

    SECTION("Calls destructor on previous value")
    {
      REQUIRE( is_called );
    }

    SECTION("Converts to null")
    {
      REQUIRE_FALSE( static_cast<bool>(optional) );
    }
  }

  SECTION("Assigning over null value")
  {
    auto optional = bpstd::optional<int>( bpstd::nullopt );

    SECTION("Converts to null")
    {
      REQUIRE_FALSE( static_cast<bool>(optional) );
    }
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::operator=( const optional& )","[assignment]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::operator=( optional&& )","[assignment]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::operator=( U&& )","[assignment]")
{
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

TEST_CASE("optional::operator->()","[observers]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::operator->() const","[observers]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::operator*() &","[observers]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::operator*() &&","[observers]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::operator*() const &","[observers]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::operator*() const &&","[observers]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::operator bool()","[observers]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value() &","[observers]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value() const &","[observers]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value() &&","[observers]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value() const &&","[observers]")
{
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value_or( U&& ) const &","[observers]")
{
  SECTION("Optional is null")
  {
    auto optional = bpstd::optional<int>();

    REQUIRE( optional.value_or(42) == 42 );
  }

  SECTION("Optional is non-null")
  {
    auto optional = bpstd::optional<int>(32);

    REQUIRE( optional.value_or(42) == 32 );
  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::value_or( U&& ) &&","[observers]")
{
  SECTION("Optional is null")
  {
    auto optional = bpstd::optional<int>();

    REQUIRE( std::move(optional).value_or(42) == 42 );
  }

  SECTION("Optional is non-null")
  {
    auto optional = bpstd::optional<int>(32);

    REQUIRE( std::move(optional).value_or(42) == 32 );
  }
}

//----------------------------------------------------------------------------
// Modifiers
//----------------------------------------------------------------------------

TEST_CASE("optional::swap( optional<T>& )","[modifiers]")
{
  SECTION("Both optionals are null")
  {
    auto op1 = bpstd::optional<int>();
    auto op2 = bpstd::optional<int>();

    op1.swap(op2);

  }

  SECTION("Both optionals are non-null")
  {
    auto value1 = 32;
    auto value2 = 64;
    auto op1 = bpstd::optional<int>(value1);
    auto op2 = bpstd::optional<int>(value2);

    op1.swap(op2);

    SECTION("Values swapped")
    {
      SECTION("op1 contains op2's value")
      {
        REQUIRE( op1.value() == value2 );
      }
      SECTION("op2 contains op1's value")
      {
        REQUIRE( op2.value() == value1 );
      }
    }

    SECTION("Optionals are non-null")
    {
      SECTION("op1 is non-null")
      {
        REQUIRE( static_cast<bool>(op1) );
      }
      SECTION("op2 is non-null")
      {
        REQUIRE( static_cast<bool>(op2) );
      }
    }
  }

  SECTION("(*this) optional is null")
  {

  }

  SECTION("other optional is null")
  {

  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::emplace( Args&&... )","[modifiers]")
{
  SECTION("Original optional is null")
  {

  }

  SECTION("Original optional is non-null")
  {

  }
}

//----------------------------------------------------------------------------

TEST_CASE("optional::emplace( std::initializer_list<U>, Args&&... )","[modifiers]")
{
  SECTION("Original optional is null")
  {

  }

  SECTION("Original optional is non-null")
  {

  }
}
