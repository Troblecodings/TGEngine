#pragma once

#define USAGE(enum_usage) // Defines the usage of an annotation within this header
#define USAGE_DOCS USAGE("DOCUMANTATION")
#define USAGE_DEBUG USAGE("DEBUG")
#define USAGE_CODE USAGE("CODE")
#define USAGE_DEPRECATION USAGE("DEPRECATION")

#define TARGET(enum_target) // Defines the target of an annotation within this header
#define TARGET_METHODE TARGET("METHODE")
#define TARGET_CODE TARGET("CODE")
#define TARGET_PARAMETER TARGET("PARAMETER")

USAGE_DOCS
USAGE_DEPRECATION
TARGET_CODE
TARGET_METHODE
#define DEPRECATED // Deprecated code or method follows

USAGE_DOCS
USAGE_DEPRECATION
TARGET_CODE
TARGET_METHODE
#define INSTEAD_USE(use) // You should use this method instead (Only used in combination with DEPRECATED)

USAGE_DOCS
TARGET_CODE
#define END_DEPRECATED // End of deprecated code

USAGE_DOCS
TARGET_METHODE
TARGET_CODE
#define UNFINISHED // Unfinished code or method follows

USAGE_DOCS
TARGET_CODE
#define END_UNFINISHED // End of unfinished code

USAGE_DOCS
TARGET_METHODE
#define SINCE(major, minor, version) // Tells since which engine version this method is in the engine
// Remember that methods as well as parameters and/or it's function can change eventhough the version may stay the same

USAGE_DOCS
TARGET_PARAMETER
#define INPUT // Defines the parameter to be an Input

USAGE_DOCS
TARGET_PARAMETER
#define OUTPUT // Defines the parameter to be an Output

USAGE_DOCS
TARGET_PARAMETER
#define OPT // Defines the parameter to be an optional input or output

USAGE_DOCS
TARGET_METHODE
TARGET_PARAMETER
#define INTERNAL // Defines a method or parameter to be internal -> This methods or parameters shouldn't be used