#[[#ifndef]]# HEADER_GUARD_${NAME.toUpperCase()}_H
#[[#define]]# HEADER_GUARD_${NAME.toUpperCase()}_H

#[[#include]]# "core/io/resource.h"
#[[#include]]# "game_common/gdscript_helpers.h"

class ${NAME} : public Resource {
    GDCLASS(${NAME}, Resource);

protected:
    static void _bind_methods() {
        //BIND_GETSET(INT, weight);
        //BIND_GETSET_RESOURCE(ItemModifierGen, item_modifier);
    }

public:

};
#endif // HEADER_GUARD_${NAME.toUpperCase()}_H
