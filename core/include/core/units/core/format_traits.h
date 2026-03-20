#pragma once

namespace units {

struct kelvin_tag {};
struct celsius_tag {};
struct fahrenheit_tag {};

template<class UnitTag>
struct unit_text;

template<class Quantity>
struct default_unit_for_quantity;

}  // namespace units
