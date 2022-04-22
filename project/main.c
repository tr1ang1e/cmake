#include "common.h"
#include <dlfcn.h>   // runtime dynamic library linkage


static void (*init)();                                     // va7044_A0_regs_init();
static void (*destroy)();                                  // va7044_A0_regs_destroy();
static bool (*get)(const char* key, RegFieldInfo* item);   // va7044_A0_regs_get(const char* pFieldName, RegFieldInfo* pItem);

void print_found_info(RegFieldInfo* pFound, bool isFound)
{
    if (isFound)
    {
        TYPE type = (pFound->mask == 0xFFFFFFFFU) ? REGISTER : FIELD;

        printf("REGISTER or FIELD found \n");
        printf("    type = %s \n", type ? "field" : "register");
        printf("    name = %s \n", pFound->name);
        printf("  adress = 0x%08X \n", pFound->adress);

        if (type)
        {
            printf("  offset = %d \n", pFound->offset);
            printf("    mask = 0x%08X \n", pFound->mask);
        }
    }
    else
    {
        printf("unknown REGISTER or FIELD name \n");
    }

    printf("\n");
}

int main(int argc, char** argv)
{
    bool result = -1;
    void* library = NULL;
    RegFieldInfo item = {0};

    do
    {
        // get library
        library = dlopen("./libregs.so", RTLD_LAZY);
        if(library == NULL)
        {
            printf("%s \n", dlerror());
            break;
        }

        init = (void (*)())dlsym(library, "va7044_A0_regs_init");         if (init == NULL) break;
        destroy = (void (*)())dlsym(library, "va7044_A0_regs_destroy");   if (init == NULL) break;
        get = (bool (*)(const char*, RegFieldInfo*))dlsym(library, "va7044_A0_regs_get");           if (init == NULL) break;

        init();
        
        result = get("_A_P1_CORE_DIG_IOCTRL_RW_DPHY_PPI_LANE1_OVR_0_7_reg", &item);
        print_found_info(&item, result);          
        
        result = get("B_I_TXALPNIBBLE_C2_OVR_VAL", &item);
        print_found_info(&item, result);          

        result = get("WRONG_NAME", &item);
        print_found_info(&item, result);

        destroy();

        result = 0;
    } while (0);
    
    if (library)
    {
        dlclose(library);
    }

    return result;
}