#pragma once

// why is this even a warning? if I didn't want to use c++14 features
// then I just wouldn't. Stop assaulting my eyes
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++14-extensions"

#include "core/units/core/dim.h"
#include "core/units/core/format_traits.h"
#include "core/units/core/quantity_base.h"
#include "core/units/core/unit_traits.h"
#include "core/units/format.h"
#include "core/units/math.h"
#include "core/units/types/angle.h"
#include "core/units/types/amount.h"
#include "core/units/types/control.h"
#include "core/units/types/current.h"
#include "core/units/types/electrical.h"
#include "core/units/types/geometry.h"
#include "core/units/types/length.h"
#include "core/units/types/luminous_intensity.h"
#include "core/units/types/mass.h"
#include "core/units/types/mechanics.h"
#include "core/units/types/number.h"
#include "core/units/types/photometry.h"
#include "core/units/types/temperature.h"
#include "core/units/types/time.h"

using namespace units;
using namespace units::literals;

#pragma clang diagnostic pop
