DESCRIPTION = "Video player and streamer - davinci edition"
HOMEPAGE = "http://www.videolan.org"
SECTION = "multimedia"

LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263"

DEPENDS = "libfribidi libtool libgcrypt gst-plugins-bad virtual/libsdl \
   libsdl-image qt4-x11-free dbus dbus-glib libxml2 gnutls tremor \
   faad2 ffmpeg flac libxpm libxinerama fluidsynth alsa-lib libdvdcss \
   libdvdread lua-native lua libidn libnotify gtk+ gnome-vfs libdc1394 \
   libraw1394 avahi libjpeg-turbo xz libmodplug mpeg2dec \
   libmtp opencv libopus orc libusb1 schroedinger taglib \
   tiff xcb-util-keysyms samba"

SRC_URI = "http://download.videolan.org/pub/videolan/${BPN}/${PV}/${BP}.tar.xz"

inherit autotools gettext

ARM_INSTRUCTION_SET = "arm"

EXTRA_OECONF = "\
    --enable-dvdread \
    --with-contrib \
    --enable-run-as-root \
    --enable-xvideo \ 
    --disable-screen --disable-caca \
    --enable-httpd --enable-vlm \
    --enable-freetype \
    --enable-sdl \ 
    --enable-png \
    --enable-live555 --enable-tremor \
    --enable-v4l2 --disable-aa --disable-faad \
    --enable-dbus \
    --without-contrib \
    --without-kde-solid \
    --disable-glx \
    --enable-realrtsp \
    ac_cv_path_MOC=${STAGING_BINDIR_NATIVE}/moc4 \
    ac_cv_path_RCC=${STAGING_BINDIR_NATIVE}/rcc4 \
    ac_cv_path_UIC=${STAGING_BINDIR_NATIVE}/uic4 \
"

PACKAGECONFIG[mad] = "--enable-mad,--disable-mad,libmad"
PACKAGECONFIG[a52] = "--enable-a52,--disable-a52,liba52"
PACKAGECONFIG[jack] = "--enable-jack,--disable-jack,jack"
PACKAGECONFIG[live555] = "--enable-live555,--disable-live555,live555"
PACKAGECONFIG[libass] = "--enable-libass,--disable-libass,libass"
PACKAGECONFIG[mkv] = "--enable-mkv,--disable-mkv,libmatroska libebml"

do_configure() {
    cp ${STAGING_DATADIR}/aclocal/libgcrypt.m4 ${S}/m4/ 
    ./bootstrap    
    gnu-configize --force
    libtoolize --force
    #autoreconf --force -i
    cp ${STAGING_DATADIR}/libtool/config.* ${S}/autotools/ || true
    oe_runconf
    rm config.log
    #sed -i -e s:-L/usr/lib:-L${STAGING_LIBDIR}/:g ${S}/vlc-config
    sed -i -e s:'$(MOC) $(DEFS) $(CPPFLAGS)':'$(MOC) $(DEFS)'\ -I${S}/include\ -DSYS_LINUX:g ${S}/modules/gui/qt4/Makefile
    sed -i -e s:'${top_builddir_slash}libtool':${TARGET_SYS}-libtool:g ${S}/doltlibtool
}

# This recipe packages vlc as a library as well, so qt4 dependencies
# can be avoided when only the library is installed.
PACKAGES =+ "libvlc"

LEAD_SONAME_libvlc = "libvlc.so.5"
FILES_libvlc = "${libdir}/lib*.so.*"

FILES_${PN} += "\
    ${bindir}/vlc \
    ${datadir}/applications \
    ${datadir}/vlc/ \
    ${datadir}/icons \
"

FILES_${PN}-dbg += "\
    ${libdir}/vlc/*/.debug \
    ${libdir}/vlc/plugins/*/.debug \
"

FILES_${PN}-staticdev += "\
    ${libdir}/vlc/plugins/*/*.a \
"
