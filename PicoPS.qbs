import qbs
import qbs.FileInfo

Project {
	minimumQbsVersion: "1.6.0"
CppApplication
{
	property string ChibiOS: "ChibiOS"

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
    "-O0",
//	"-flto=4",
//  "-fdata-sections",
		"-ffunction-sections",
//	"-fshort-enums",
//	"-ffreestanding",
		"-Wno-unused-function",
		"-Wno-maybe-uninitialized"
	]
	Group {	name: "Linker files"
		prefix: "SW4STM32/STM32746G_DISCOVERY/"
		fileTags: ["linkerscript"]
		files: "STM32F746NGHx_FLASH_ETH.ld"
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
	]
  Modbus { condition: true }
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
         "main.cpp",
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

