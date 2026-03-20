#pragma once

#include "core/units/core/format_traits.h"
#include "core/units/core/quantity_base.h"

// this blows D:
#define UNITS_DEFINE_LITERAL_PAIR(suffix, expression)                                          \
  constexpr auto operator"" suffix(long double value) -> decltype(expression) {                \
    return expression;                                                                         \
  }                                                                                            \
  constexpr auto operator"" suffix(unsigned long long value)                                   \
      -> decltype(expression) {                                                                \
    return expression;                                                                         \
  }

#define UNITS_DEFINE_TYPED_LITERAL(QuantityType, short_name, tag_name)                         \
  UNITS_DEFINE_LITERAL_PAIR(_##short_name, QuantityType::from<tag_name>(static_cast<double>(value)));

#define UNITS_DECLARE_TAG_AND_TRAITS(DimType, short_name, long_name, tag_name, scale_num,     \
                                     scale_den)                                                \
  struct tag_name {};                                                                          \
  UNITS_DEFINE_MULTIPLICATIVE_TRAITS(tag_name, DimType, scale_num, scale_den)

#define UNITS_DECLARE_TABLE_GETTER(context, short_name, long_name, tag_name, ...)              \
  constexpr double short_name() const { return this->template as<tag_name>(); }                \
  constexpr double long_name() const { return this->template as<tag_name>(); }

#define UNITS_DECLARE_TYPED_LITERAL_FOR(QuantityType, short_name, long_name, tag_name, ...)    \
  UNITS_DEFINE_TYPED_LITERAL(QuantityType, short_name, tag_name)

#define UNITS_DEFINE_UNIT_TEXT_TRAIT(context, short_name, long_name, tag_name, ...)            \
  template<>                                                                                   \
  struct unit_text<tag_name> {                                                                 \
    static const char* symbol() { return #short_name; }                                        \
    static const char* name() { return #long_name; }                                           \
  };

#define UNITS_DEFINE_DEFAULT_UNIT_TRAIT(QuantityType, UnitTag)                                 \
  template<>                                                                                   \
  struct default_unit_for_quantity<QuantityType> {                                             \
    typedef UnitTag type;                                                                      \
  };

#define UNITS_DEFINE_TABLE_LITERALS(QuantityType, UnitTableMacro)                              \
  UnitTableMacro(UNITS_DECLARE_TYPED_LITERAL_FOR, QuantityType)

#define UNITS_DEFINE_TABLE_TAGS_AND_TRAITS(DimType, UnitTableMacro)                            \
  UnitTableMacro(UNITS_DECLARE_TAG_AND_TRAITS, DimType)

#define UNITS_DEFINE_SEMANTIC_QUANTITY(ClassName, DimType, UnitTableMacro)                     \
  class ClassName : public quantity_base<ClassName, DimType> {                                 \
   public:                                                                                     \
    typedef quantity_base<ClassName, DimType> base_type;                                       \
    typedef DimType dimension;                                                                 \
                                                                                               \
    constexpr explicit ClassName(double canonical_value = 0.0)                                 \
        : base_type(canonical_value) {}                                                        \
                                                                                               \
    template<class OtherQuantity>                                                              \
    constexpr ClassName(const OtherQuantity& other,                                            \
                        typename std::enable_if<                                               \
                            std::is_same<DimType,                                              \
                                         typename OtherQuantity::dimension>::value,            \
                            int>::type = 0)                                                    \
        : base_type(other.canonical_value()) {}                                                \
                                                                                               \
    static constexpr ClassName from_canonical(double value) {                                  \
      return ClassName(value);                                                                 \
    }                                                                                          \
                                                                                               \
    template<class UnitTag>                                                                    \
    static constexpr ClassName from(double value) {                                            \
      return ClassName(base_type::template to_canonical<UnitTag>(value));                      \
    }                                                                                          \
                                                                                               \
    UnitTableMacro(UNITS_DECLARE_TABLE_GETTER, _)                                              \
  };                                                                                           \
                                                                                               \
  template<>                                                                                   \
  struct quantity_for_dimension<DimType> {                                                     \
    typedef ClassName type;                                                                    \
  }

#define UNITS_DEFINE_SEMANTIC_ALIAS(ClassName, DimType, UnitTableMacro)                        \
  class ClassName : public quantity_base<ClassName, DimType> {                                 \
   public:                                                                                     \
    typedef quantity_base<ClassName, DimType> base_type;                                       \
    typedef DimType dimension;                                                                 \
                                                                                               \
    constexpr explicit ClassName(double canonical_value = 0.0)                                 \
        : base_type(canonical_value) {}                                                        \
                                                                                               \
    template<class OtherQuantity>                                                              \
    constexpr ClassName(const OtherQuantity& other,                                            \
                        typename std::enable_if<                                               \
                            std::is_same<DimType,                                              \
                                         typename OtherQuantity::dimension>::value,            \
                            int>::type = 0)                                                    \
        : base_type(other.canonical_value()) {}                                                \
                                                                                               \
    static constexpr ClassName from_canonical(double value) {                                  \
      return ClassName(value);                                                                 \
    }                                                                                          \
                                                                                               \
    template<class UnitTag>                                                                    \
    static constexpr ClassName from(double value) {                                            \
      return ClassName(base_type::template to_canonical<UnitTag>(value));                      \
    }                                                                                          \
                                                                                              \
    UnitTableMacro(UNITS_DECLARE_TABLE_GETTER, _)                                              \
  }

#define UNITS_DEFINE_QUANTITY_TYPE(ClassName, DimType, UnitTableMacro, DefaultUnitTag)         \
  UNITS_DEFINE_SEMANTIC_QUANTITY(ClassName, DimType, UnitTableMacro);                          \
  UnitTableMacro(UNITS_DEFINE_UNIT_TEXT_TRAIT, _)                                              \
  UNITS_DEFINE_DEFAULT_UNIT_TRAIT(ClassName, DefaultUnitTag)
