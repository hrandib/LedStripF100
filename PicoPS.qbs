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
    prefix: "c:/Projects/ChibiOS_based/PicoPS/ChibiOS/os/common/startup/ARMCMx/compilers/GCC/ld/"
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
      "compilers/GCC/chcoreasm_v6m.S",
      "compilers/GCC/chtypes.h",
      "chcore_v6m.c",
      "chcore.c",
    ]
  }
  Group { name: "Test sources"
		files: [
     ]
	}
	Group { name: "No LTO"
		cpp.commonCompilerFlags: [
			"-fno-lto"
		]
		files: [
		]
	}
	Group { name: "LCD"
		files:
		[
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

