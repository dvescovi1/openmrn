-include ../openmrnpath.mk
ifndef OPENMRNPATH
OPENMRNPATH := $(realpath ..)
endif
export OPENMRNPATH


SUBDIRS = \
	async_blink \
	blink_raw \
	can_eth \
	clinic_app \
	hub \
	io_board \
	js_hub \
	js_client \
	js_cdi_server \
	memconfig_utils \
	send_datagram \
	simple_client \
	tractionproxy \
	train \
	tcp_blink_client \
	usb_can \
    time_client \
    time_server


include $(OPENMRNPATH)/etc/recurse.mk
