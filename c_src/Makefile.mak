all:
	vcbuild /platform:x64 /logcommands c_src/erloci_lib/erloci_lib.vcproj debug
	vcbuild /platform:x64 /logcommands c_src/erloci_drv/erloci_drv.vcproj debug
	
clean:
	vcbuild /clean /logcommands c_src/erloci_drv/erloci_drv.vcproj debug
	vcbuild /clean /logcommands c_src/erloci_lib/erloci_lib.vcproj debug
