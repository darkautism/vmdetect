#include <node_api.h>
#include <string.h>
#include <stdio.h>

static void vmdetect_cpuid(int i, int *regs) {
	asm volatile("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
		: "a" (i), "c" (0));
}

int vmdetect_vmware_port() {
	long long retcode;
	asm volatile ( "inl 	%%dx,%%eax\n" : "=b"(retcode)
		: "a"(0x564D5868), "0"(0xFFFFFFFF), "c"(10),"d"(0x5658):"memory" );
	return( 0x564D5868==retcode);
}

int vmdetect_vmexit() {
    unsigned long long ret, ret2;
    unsigned eax, edx;
    __asm__ volatile("rdtsc" : "=a" (eax), "=d" (edx));
    ret  = ((unsigned long long)eax) | (((unsigned long long)edx) << 32);
    __asm__ volatile("cpuid" : /* no output */ : "a"(0x00));
    __asm__ volatile("rdtsc" : "=a" (eax), "=d" (edx));
    ret2  = ((unsigned long long)eax) | (((unsigned long long)edx) << 32);
    return 500<(ret2 - ret);
}

int vmdetect_has_hypervisor() {
	int regs[4];
	vmdetect_cpuid(1, regs);
	return((regs[2] >> 31) & 0x1);
}

napi_value isHyper(napi_env env, napi_callback_info info)
{
	napi_value result;
	napi_get_boolean(env, vmdetect_has_hypervisor(), &result);
    return result;
}

napi_value hyperVenderId(napi_env env, napi_callback_info info)
{
	char hyper_vendor_id[16];
	size_t length = 0;
	vmdetect_cpuid(0x40000000, (int *)hyper_vendor_id);
	length = strlen(hyper_vendor_id+4);
    napi_status status;
    napi_value result;
	status = napi_create_string_latin1( env, hyper_vendor_id+4, length, &result);
	if(status != napi_ok ) {
		napi_throw_error(env, 0, "napi_create_string_utf8 failed.");
        status = napi_get_undefined(env, &result);
	}
    return result;
}

void Init(napi_env env, napi_value exports, napi_value module, void* priv)
{
    napi_status status;
    napi_property_descriptor desc[] =
        {
        	{ "hyperVenderId", 0, hyperVenderId, 0, 0, 0, napi_default, 0 },
        	{ "isHyper", 0, isHyper, 0, 0, 0, napi_default, 0 }
        };
    status = napi_define_properties(env, exports, 2, desc);
    if (status != napi_ok) return;
}

NAPI_MODULE(addon, Init)