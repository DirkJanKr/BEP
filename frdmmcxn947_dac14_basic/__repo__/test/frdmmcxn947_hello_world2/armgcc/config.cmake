# config to select component, the format is CONFIG_USE_${component}
# Please refer to cmake files below to get available components:
#  ${SdkRootDirPath}/devices/MCXN947/all_lib_device.cmake

set(CONFIG_COMPILER gcc)
set(CONFIG_TOOLCHAIN armgcc)
set(CONFIG_USE_COMPONENT_CONFIGURATION false)
set(CONFIG_USE_utility_debug_console_lite true)
set(CONFIG_USE_utility_assert_lite true)
set(CONFIG_USE_driver_gpio true)
set(CONFIG_USE_driver_common true)
set(CONFIG_USE_driver_clock true)
set(CONFIG_USE_driver_reset true)
set(CONFIG_USE_device_MCXN947_CMSIS true)
set(CONFIG_USE_device_MCXN947_startup true)
set(CONFIG_USE_driver_lpuart true)
set(CONFIG_USE_driver_port true)
set(CONFIG_USE_component_lpuart_adapter true)
set(CONFIG_USE_component_lists true)
set(CONFIG_USE_utilities_misc_utilities true)
set(CONFIG_USE_driver_mcx_spc true)
set(CONFIG_USE_CMSIS_Include_core_cm true)
set(CONFIG_USE_device_MCXN947_system true)
set(CONFIG_USE_driver_lpflexcomm true)
set(CONFIG_CORE cm33)
set(CONFIG_DEVICE MCXN947)
set(CONFIG_BOARD frdmmcxn947)
set(CONFIG_KIT frdmmcxn947)
set(CONFIG_DEVICE_ID MCXN947)
set(CONFIG_FPU SP_FPU)
set(CONFIG_DSP DSP)
set(CONFIG_CORE_ID cm33_core0)
