#Set Default DDR config file
ifeq ($(Z),1)
	DDR_CON ?= imx8dm_dcd_emul
else
	DDR_CON ?= imx8dm_dcd_1.6GHz
endif

OBJS += $(OUT)/board/mx8dm_$(B)/board.o \
		$(OUT)/board/board_common.o \
		$(OUT)/board/pmic.o

ifneq ($(HW), SIMU)
    OBJS += $(OUT)/board/board.o
endif

DCDH += $(SRC)/board/mx8dm_$(B)/dcd/$(DDR_CON).h \
		$(SRC)/board/mx8dm_$(B)/dcd/dcd.h \
		$(SRC)/board/mx8dm_$(B)/dcd/$(DDR_CON)_retention.h \
		$(SRC)/board/mx8dm_$(B)/dcd/dcd_retention.h

RSRC_MD += $(SRC)/board/mx8dm_$(B)/resource.txt

CTRL_MD += $(SRC)/board/mx8dm_$(B)/control.txt

DIRS += $(OUT)/board/mx8dm_$(B)

