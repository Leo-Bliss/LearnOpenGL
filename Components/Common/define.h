#pragma once

#define SPDefine(className)								\
class className;										\
using SP##className = std::shared_ptr<className>;		\