#ifndef DETAIL_OPTIONAL_INL
#define DETAIL_OPTIONAL_INL

namespace bpstd {

  //--------------------------------------------------------------------------
  // Constructor / Destructor
  //--------------------------------------------------------------------------

  template<typename T>
  inline constexpr optional<T>::optional()
    : m_has_value(false)
  {

  }

  template<typename T>
  inline constexpr optional<T>::optional( nullopt_t )
    : optional()
  {

  }

  //--------------------------------------------------------------------------

  template<typename T>
  inline optional<T>::optional( const optional& other )
    : m_has_value(other.m_has_value)
  {
    if(m_has_value)
    {
      new (val()) value_type( *other.val() );
    }
  }

  template<typename T>
  inline optional<T>::optional( optional&& other )
    : m_has_value(other.m_has_value)
  {
    if(m_has_value)
    {
      new (val()) value_type( std::move(*other.val()) );
    }
  }

  //--------------------------------------------------------------------------

  template<typename T>
  inline optional<T>::optional( const value_type& value )
    : m_has_value(true)
  {
    new (val()) value_type(value);
  }


  template<typename T>
  inline optional<T>::optional( value_type&& value )
    : m_has_value(true)
  {
    new (val()) value_type(std::move(value));
  }

  //------------------------------------------------------------------------

  template<typename T>
  template<typename...Args>
  inline optional<T>::optional( in_place_t,
                                Args&&... args )
    : m_has_value(true)
  {
    new (val()) value_type( std::forward<Args>(args)... );
  }


  template<typename T>
  template<typename U, typename...Args>
  inline optional<T>::optional( in_place_t,
                                std::initializer_list<U> ilist,
                                Args&&... args )
    : m_has_value(true)
  {
    new (val()) value_type( ilist, std::forward<Args>(args)... );
  }

  //--------------------------------------------------------------------------

  template<typename T>
  inline optional<T>::~optional()
  {
    destruct();
  }

  //--------------------------------------------------------------------------
  // Assignment
  //--------------------------------------------------------------------------

  template<typename T>
  inline optional<T>& optional<T>::operator=( nullopt_t )
  {
    destruct();
    return (*this);
  }

  template<typename T>
  inline optional<T>& optional<T>::operator=( const optional& other )
  {
    if(m_has_value && other.m_has_value) {
      *val() = *other.val();
    } else if( m_has_value ) {
      destruct();
    } else if( other.m_has_value ) {
      new (val()) value_type( *other.val() );
      m_has_value = true;
    }
    return (*this);
  }

  template<typename T>
  inline optional<T>& optional<T>::operator=( optional&& other )
  {
    if(m_has_value && other.m_has_value) {
      *val() = std::move( *other.val() );
    } else if( m_has_value ) {
      destruct();
    } else if( other.m_has_value ) {
      new (val()) value_type( std::move( *other.val() ) );
      m_has_value = true;
    }
    return (*this);
  }

  template<typename T>
  template<typename U, typename>
  inline optional<T>& optional<T>::operator=( U&& value )
  {
    if(m_has_value) {
      *val() = std::forward<U>(value);
    } else {
      new (val()) value_type( std::forward<U>(value) );
      m_has_value = true;
    }
    return (*this);
  }

  //--------------------------------------------------------------------------
  // Observers
  //--------------------------------------------------------------------------

  template<typename T>
  inline constexpr optional<T>::operator bool()
    const noexcept
  {
    return m_has_value;
  }

  //--------------------------------------------------------------------------

  template<typename T>
  inline typename optional<T>::value_type*
    optional<T>::operator->()
    noexcept
  {
    return val();
  }

  template<typename T>
  inline constexpr const typename optional<T>::value_type*
    optional<T>::operator->()
    const noexcept
  {
    return val();
  }

  //--------------------------------------------------------------------------

  template<typename T>
  inline typename optional<T>::value_type&
    optional<T>::operator*()
    & noexcept
  {
    return *val();
  }

  template<typename T>
  inline typename optional<T>::value_type&&
    optional<T>::operator*()
    && noexcept
  {
    return std::move(*val());
  }

  //--------------------------------------------------------------------------

  template<typename T>
  inline constexpr const typename optional<T>::value_type&
    optional<T>::operator*()
    const & noexcept
  {
    return *val();
  }

  template<typename T>
  inline constexpr const typename optional<T>::value_type&&
    optional<T>::operator*()
    const && noexcept
  {
    return std::move(*val());
  }

  //--------------------------------------------------------------------------

  template<typename T>
  inline typename optional<T>::value_type&
    optional<T>::value()
    &
  {
    return bool(*this) ? *val() : throw bad_optional_access();
  }

  template<typename T>
  inline constexpr const typename optional<T>::value_type&
    optional<T>::value()
    const &
  {
    return bool(*this) ? *val() : throw bad_optional_access();
  }

  //--------------------------------------------------------------------------

  template<typename T>
  inline typename optional<T>::value_type&&
    optional<T>::value()
    &&
  {
    return bool(*this) ? std::move(*val()) : throw bad_optional_access();
  }

  template<typename T>
  inline constexpr const typename optional<T>::value_type&&
    optional<T>::value()
    const &&
  {
    return bool(*this) ? std::move(*val()) : throw bad_optional_access();
  }

  //--------------------------------------------------------------------------

  template<typename T>
  template<typename U>
  inline constexpr typename optional<T>::value_type
    optional<T>::value_or( U&& default_value )
    const&
  {
    return bool(*this) ? *val() : std::forward<U>(default_value);
  }

  template<typename T>
  template<typename U>
  inline typename optional<T>::value_type
    optional<T>::value_or( U&& default_value )
    &&
  {
    return bool(*this) ? *val() : std::forward<U>(default_value);
  }

  //--------------------------------------------------------------------------
  // Modifiers
  //--------------------------------------------------------------------------

  template<typename T>
  inline void optional<T>::swap( optional<T>& other )
  {
    using std::swap;

    if( m_has_value && other.m_has_value ){
      swap(*val(),*other.val());
    } else if( m_has_value ) {
      other = std::move(*this);
    } else if( other.m_has_value ) {
      *this = std::move(other);
    }
  }

  //--------------------------------------------------------------------------

  template<typename T>
  template<typename...Args>
  inline void optional<T>::emplace( Args&&...args )
  {
    destruct();
    new (val()) value_type( std::forward<Args>(args)... );
    m_has_value = true;
  }

  template<typename T>
  template<typename U, typename...Args>
  inline void optional<T>::emplace( std::initializer_list<U> ilist, Args&&...args )
  {
    // to do
  }

  //--------------------------------------------------------------------------
  // Private Member Functions
  //--------------------------------------------------------------------------

  template<typename T>
  inline constexpr T* optional<T>::val() const noexcept
  {
    return reinterpret_cast<T*>( const_cast<storage_type*>(&m_value) );
  }

  template<typename T>
  inline void optional<T>::destruct()
    const
  {
    if(!std::is_trivially_destructible<T>::value  && m_has_value) {
      val()->~T();
      m_has_value = false;
    }
  }

} // namespace bpstd

#endif /* DETAIL_OPTIONAL_INL */
