import qbs
import qbs.FileInfo

Project {
	minimumQbsVersion: "1.6.0"
CppApplication
{
  property string ChibiOS: "ChibiOS/"

	type: ["application", "printsize"]
	consoleApplication: true
//	cpp.optimization: "small"
	cpp.debugInformation: false
//	cpp.enableExceptions: true
	cpp.positionIndependentCode: false
//	cpp.enableRtti: true
	cpp.cLanguageVersion: "c11"
//cpp.cxxLanguageVersion: "gnu++14"
	cpp.cxxFlags: ["-std=gnu++14"]
	cpp.executableSuffix: ".elf"
	cpp.defines:
	[
		"_GLIBCXX_USE_C99",
		"_GLIBCXX_HAVE_BROKEN_VSWPRINTF"
	]
	cpp.driverFlags:
	[
		"-mthumb",
		"-mcpu=cortex-m3",
		"-mtune=cortex-m3",
		"-ggdb3",
		"-fmessage-length=0",
		/*"-Wa,--defsym,THUMB2=1"*/
		//"-nostdlib", "-nodefaultlibs"
	]
	cpp.commonCompilerFlags:
	[
    "-O2",
//	"-flto=4",
//  "-fdata-sections",
		"-ffunction-sections",
//	"-fshort-enums",
//	"-ffreestanding",
		"-Wno-unused-function",
		"-Wno-maybe-uninitialized"
	]
	Group {	name: "Linker files"
    prefix: ChibiOS + "os/common/startup/ARMCMx/compilers/GCC/ld/"
		fileTags: ["linkerscript"]
    files: "STM32F103x8.ld"
	}
	cpp.linkerFlags:
	[
		"--gc-sections",
		"--Map=c:/Projects/output.map"
//	"-nostartfiles",
//	"--specs=nosys.specs",
//	"--specs=nano.specs",
//	"-lm",
//	"-lnosys","-lgcc","-lc", "-lstdc++",
		]
	cpp.includePaths:
	[
    "config",
    //Startup
    ChibiOS + "os/common/startup/ARMCMx/compilers/GCC/",
    ChibiOS + "os/common/startup/ARMCMx/devices/STM32F1xx",
    ChibiOS + "os/common/ext/CMSIS/include",
    ChibiOS + "os/common/ext/CMSIS/ST/STM32F1xx",
    //Port
    ChibiOS + "os/common/ports/ARMCMx",
    ChibiOS + "os/common/ports/ARMCMx/compilers/GCC",
    //RT
    ChibiOS + "os/rt/include",
    ChibiOS + "os/common/oslib/include",
    //OSAL
    ChibiOS + "os/hal/osal/rt",
    //HAL
    ChibiOS + "os/hal/include",
    //Platform
    ChibiOS + "os/hal/ports/common/ARMCMx",
    ChibiOS + "os/hal/ports/STM32/STM32F1xx",
    //Drivers
    ChibiOS + "os/hal/ports/STM32/LLD/CANv1",
    ChibiOS + "os/hal/ports/STM32/LLD/DACv1",
    ChibiOS + "os/hal/ports/STM32/LLD/DMAv1",
    ChibiOS + "os/hal/ports/STM32/LLD/EXTIv1",
    ChibiOS + "os/hal/ports/STM32/LLD/GPIOv1",


	]
  Group { name: "Startup"
    prefix: ChibiOS + "os/common/startup/ARMCMx/compilers/GCC/"
    files: [
      "crt0_v7m.S",
      "crt1.c",
      "vectors.c"
    ]
 	}
  Group { name: "Port"
    prefix: ChibiOS + "os/common/ports/ARMCMx/"
    files: [
      "compilers/GCC/chcoreasm_v7m.S",
      "compilers/GCC/chtypes.h",
      "chcore_v7m.c",
      "chcore.c",
    ]
  }
  Group { name: "Platform"
    prefix: ChibiOS + "os/hal/ports/STM32/STM32F1xx/"
    files: [
      "hal_ext_lld_isr.c",
      "hal_adc_lld.c"
    ]
  }
  Group { name: "Drivers"
    prefix: ChibiOS + "os/hal/ports/STM32/LLD/"
    files: [
      "CANv1/hal_can_lld.h",
      "CANv1/hal_can_lld.c",
      "DACv1/hal_dac_lld.h",
      "DACv1/hal_dac_lld.c",
      "DMAv1/stm32_dma.h",
      "DMAv1/stm32_dma.c",
      "EXTIv1/hal_ext_lld.h",
      "EXTIv1/hal_ext_lld.c",
      "GPIOv1/hal_pal_lld.h",
      "GPIOv1/hal_pal_lld.c"
    ]
  }


  Group { name: "RT"
    prefix: ChibiOS + "os/"
    files: [
      "rt/src/chsys.c",
      "rt/src/chdebug.c",
      "rt/src/chtrace.c",
      "rt/src/chvt.c",
      "rt/src/chschd.c",
      "rt/src/chthreads.c",
      "rt/src/chtm.c",
      "rt/src/chstats.c",
      "rt/src/chregistry.c",
      "rt/src/chsem.c",
      "rt/src/chmtx.c",
      "rt/src/chcond.c",
      "rt/src/chevents.c",
      "rt/src/chmsg.c",
      "rt/src/chdynamic.c",
      "common/oslib/src/chmboxes.c",
      "common/oslib/src/chmemcore.c",
      "common/oslib/src/chheap.c",
      "common/oslib/src/chmempools.c"
    ]
  }
  Group { name: "OSAL"
		files: [
      ChibiOS + "os/hal/osal/rt/osal.h",
      ChibiOS + "os/hal/osal/rt/osal.c"
    ]
	}
  Group { name: "HAL"
    prefix: ChibiOS + "os/hal/src/"
    files: [
      "hal.c",
      "hal_buffers.c",
      "hal_queues.c",
      "hal_mmcsd.c",
      "hal_adc.c",
      "hal_can.c",
      "hal_dac.c",
      "hal_ext.c",
      "hal_gpt.c",
      "hal_i2c.c",
      "hal_i2s.c",
      "hal_icu.c",
      "hal_mac.c",
      "hal_mmc_spi.c",
      "hal_pal.c",
      "hal_pwm.c",
      "hal_qspi.c",
      "hal_rtc.c",
      "hal_sdc.c",
      "hal_serial.c",
      "hal_serial_usb.c",
      "hal_spi.c",
      "hal_st.c",
      "hal_uart.c",
      "hal_usb.c",
      "hal_wdg.c"
    ]
  }
	Group { name: "No LTO"
		cpp.commonCompilerFlags: [
			"-fno-lto"
		]
		files: [
		]
	}

	Group {	name: "Common"
		files:
    [
         "main.c"
     ]
		excludeFiles:
		[
			"**/*_res.c",
			"**/*_conf_template.c",
			"**/*_conf_template.h",
			"**/*.html",
		]
	}
	Rule
	{
		id: size
		inputs: ["application"]
		Artifact {
			fileTags: ["printsize"]
			filePath: "-"
		}
		prepare: {
			var sizePath = "c:/Tools/gccarm_5.3/bin/arm-none-eabi-size.exe";
			var args = [input.filePath];
			var cmd = new Command(sizePath, args);
			cmd.description = "File size: " + FileInfo.fileName(input.filePath);
			cmd.highlight = "linker";
			return cmd;
		}
	}
}
}

