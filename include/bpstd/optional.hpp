/*
 * optional.hpp
 *
 *  Created on: Sep 18, 2016
 *      Author: matthewrodusek
 */

#ifndef BPSTD_OPTIONAL_HPP
#define BPSTD_OPTIONAL_HPP

#include <initializer_list>
#include <type_traits>
#include <memory>
#include <utility>

namespace bpstd {

  class bad_optional_access : public std::logic_error
  {
  public:
    bad_optional_access() : std::logic_error("bpstd::bad_optional_access"){}

  };

  struct nullopt_t{};

  struct in_place_t{};

  constexpr nullopt_t nullopt   = nullopt_t{};
  constexpr in_place_t in_place = in_place_t{};

  ////////////////////////////////////////////////////////////////////////////
  /// \class bpstd::optional
  ///
  /// \brief The class template optional manages an optional contained value,
  ///        i.e. a value that may or may not be present.
  ///
  /// A common use case for optional is the return value of a function that
  /// may fail. As opposed to other approaches, such as std::pair<T,bool>,
  /// optional handles expensive to construct objects well and is more
  /// readable, as the intent is expressed explicitly.
  ///
  /// Any instance of optional<T> at any given point in time either contains a
  /// value or does not contain a value.
  ///
  /// If an optional<T> contains a value, the value is guaranteed to be
  /// allocated as part of the optional object footprint, i.e. no dynamic
  /// memory allocation ever takes place. Thus, an optional object models an
  /// object, not a pointer, even though the operator*() and operator->()
  /// are defined.
  ///
  /// When an object of type optional<T> is contextually converted to bool,
  /// the conversion returns true if the object contains a value and false if
  /// it does not contain a value.
  ///
  /// The optional object contains a value in the following conditions:
  /// - The object is initialized with a value of type T
  /// - The object is assigned from another optional that contains a value.
  /// - The object does not contain a value in the following conditions:
  /// - The object is default-initialized.
  /// - The object is initialized with a value of nullopt_t or an optional
  ///   object that does not contain a value.
  /// - The object is assigned from a value of nullopt_t or from an optional
  ///   that does not contain a value
  ////////////////////////////////////////////////////////////////////////////
  template<typename T>
  class optional final
  {
    //------------------------------------------------------------------------
    // Public Member Types
    //------------------------------------------------------------------------
  public:

    using value_type = T;

    //------------------------------------------------------------------------
    // Constructor / Destructor
    //------------------------------------------------------------------------
  public:

    constexpr optional();

    constexpr optional( nullopt_t );

    optional( const optional& other );

    optional( optional&& other );

    optional( const value_type& value );

    optional( value_type&& value );

    template<typename...Args>
    explicit optional( in_place_t, Args&&... args );

    template<typename U, typename...Args>
    explicit optional( in_place_t,
                       std::initializer_list<U> ilist,
                       Args&&...args );

    ~optional();


    //------------------------------------------------------------------------
    // Assignment
    //------------------------------------------------------------------------
  public:

    optional& operator=( nullopt_t );
    optional& operator=( const optional& other );
    optional& operator=( optional&& other );
    template<typename U, typename = typename std::is_same<typename std::decay<U>::type, T>::value>
    optional& operator=( U&& value );

    //------------------------------------------------------------------------
    // Observers
    //------------------------------------------------------------------------
  public:

    value_type* operator->() noexcept;

    constexpr const value_type* operator->() const noexcept;

    value_type& operator*() & noexcept;

    value_type&& operator*() && noexcept;

    constexpr const value_type& operator*() const& noexcept;

    constexpr const value_type&& operator*() const&& noexcept;

    /// \brief Checks whether \c *this contains a value
    ///
    /// \return \c true if \c *this contains a value, \c false if \c *this
    ///         does not contain a value
    constexpr explicit operator bool() const noexcept;

    //------------------------------------------------------------------------

    /// \brief Returns the contained value.
    ///
    /// Equivalent to
    /// \code
    /// return bool(*this) ? *val : throw bad_optional_access();
    /// \endcode
    ///
    /// \throws #bad_optional_access if \c *this does not contain a value.
    ///
    /// \return the value of \c *this
    value_type& value() &;

    /// \copydoc value() &
    constexpr const value_type& value() const &;

    /// \brief Returns the contained value.
    ///
    /// Equivalent to
    /// \code
    /// return bool(*this) ? std::move(*val) : throw bad_optional_access();
    /// \endcode
    ///
    /// \throws #bad_optional_access if \c *this does not contain a value.
    ///
    /// \return the value of \c *this
    value_type&& value() &&;

    /// \copydoc value() &&
    constexpr const value_type&& value() const &&;

    //------------------------------------------------------------------------

    /// \brief Returns the contained value if \c *this has a value,
    ///        otherwise returns \p default_value.
    ///
    /// \param default_value the value to use in case \c *this is empty
    /// \return the value to use in case \c *this is empty
    template<typename U>
    constexpr value_type value_or( U&& default_value ) const &;

    /// \copydoc value_or( U&& )
    template<typename U>
    value_type value_or( U&& default_value ) &&;

    //------------------------------------------------------------------------
    // Modifiers
    //------------------------------------------------------------------------
  public:

    /// \brief Swaps the contents with those of other.
    ///
    /// \param other the optional object to exchange the contents with
    void swap( optional& other );

    /// \brief Constructs the contained value in-place.
    ///
    /// If \c *this already contains a value before the call, the contained
    /// value is destroyed by calling its destructor.
    ///
    /// \param args... the arguments to pass to the constructor
    template<typename...Args>
    void emplace( Args&&...args );

    /// \brief Constructs the contained value in-place.
    ///
    /// If \c *this already contains a value before the call, the contained
    /// value is destroyed by calling its destructor.
    ///
    /// \param ilist   the initializer list to pass to the constructor
    /// \param args... the arguments to pass to the constructor
    template<typename U,typename...Args >
    void emplace( std::initializer_list<U> ilist, Args&&...args );

    //------------------------------------------------------------------------
    // Private Member Types
    //------------------------------------------------------------------------
  private:

    using storage_type = typename std::aligned_storage<sizeof(T),alignof(T)>::type;

    //------------------------------------------------------------------------
    // Private Members
    //------------------------------------------------------------------------
  private:

    storage_type m_value;      ///< The value of this optional
    mutable bool m_has_value;  ///< Whether or not the optional has a value

    //------------------------------------------------------------------------
    // Private Member Functions
    //------------------------------------------------------------------------
  private:

    /// \brief Gets a pointer to the value type
    ///
    /// \return the pointer
    constexpr T* val() const noexcept;

    /// \brief Destructs the value of the optional if it is not trivially destructible
    void destruct() const;

  };

} // namespace bpstd

#include "detail/optional.inl"

#endif /* BPSTD_OPTIONAL_HPP */
