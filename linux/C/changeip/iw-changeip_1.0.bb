DESCRIPTION = "Change IPv4 Address"
SECTION = "innowell_application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://changeip.c"

S = "${WORKDIR}"

PN = "iw-changeip"
PV = "1.0"
PR = "r0"
PE = "0"

do_compile() {
	${CC} ${LDFLAGS} changeip.c -o ${PN}_${PV}_${PR}.out 
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 ${PN}_${PV}_${PR}.out ${D}${bindir}
}
