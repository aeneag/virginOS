#Aen @2022.04.05
#公众号：技术乱舞
#https://aeneag.xyz
#filename:
#description:主控自动化编译配置文件
MAKEFLAGS = -sR

MKDIR = mkdir
RMDIR = rmdir
CP = cp
CD = cd
DD = dd
RM = rm
LKIMG = ./lmoskrlimg -m k
VM = qemu-system-x86_64
DBUGVM = bochs -q
IMGTOVDI = qemu-img convert -f raw -O vdi
IMGTOVMDK = qemu-img convert -f raw -O vmdk
MAKE = make
X86BARD = -f ./Makefile.x86

VVMRLMOSFLGS = -C $(BUILD_PATH) -f vbox.mkf

VBOXVMFLGS = -C $(VM_PATH) -f vbox.mkf
VMFLAGES = -smp 4 -hda $(VMDKFNAME) -m 256 -enable-kvm

IMGSECTNR = 4096000
PHYDSK = /dev/sdb
VDIFNAME = virginOS.vdi
VMDKFNAME = virginOS.img
KRNLEXCIMG = virginos.bin

LOGOFILE = logo.bmp background.bmp

START_Aen001 =  AenV9os005.bmp AenV9os006.bmp AenV9os007.bmp AenV9os008.bmp AenV9os009.bmp AenV9os010.bmp AenV9os011.bmp AenV9os012.bmp AenV9os013.bmp AenV9os014.bmp AenV9os015.bmp
START_Aen002 =  AenV9os016.bmp AenV9os017.bmp AenV9os018.bmp AenV9os019.bmp AenV9os020.bmp AenV9os021.bmp AenV9os022.bmp AenV9os023.bmp AenV9os024.bmp AenV9os025.bmp
START_Aen003 =  AenV9os026.bmp AenV9os027.bmp AenV9os028.bmp AenV9os029.bmp AenV9os030.bmp AenV9os031.bmp AenV9os032.bmp AenV9os033.bmp AenV9os034.bmp AenV9os035.bmp
START_Aen004 =  AenV9os036.bmp AenV9os037.bmp AenV9os038.bmp AenV9os039.bmp AenV9os040.bmp AenV9os041.bmp AenV9os042.bmp AenV9os043.bmp AenV9os044.bmp AenV9os045.bmp
START_Aen005 =  AenV9os046.bmp AenV9os047.bmp AenV9os048.bmp AenV9os049.bmp AenV9os050.bmp AenV9os051.bmp AenV9os052.bmp AenV9os053.bmp AenV9os054.bmp AenV9os055.bmp
START_Aen006 =  AenV9os056.bmp AenV9os057.bmp AenV9os058.bmp AenV9os059.bmp AenV9os060.bmp AenV9os061.bmp AenV9os062.bmp AenV9os063.bmp AenV9os064.bmp AenV9os065.bmp
START_Aen007 =  AenV9os066.bmp AenV9os067.bmp AenV9os068.bmp AenV9os069.bmp AenV9os070.bmp AenV9os071.bmp AenV9os072.bmp AenV9os073.bmp AenV9os074.bmp AenV9os075.bmp
START_Aen008 =  AenV9os076.bmp AenV9os077.bmp AenV9os078.bmp AenV9os079.bmp AenV9os080.bmp AenV9os081.bmp AenV9os082.bmp AenV9os083.bmp AenV9os084.bmp AenV9os085.bmp
START_Aen009 =  AenV9os086.bmp AenV9os087.bmp AenV9os088.bmp AenV9os089.bmp AenV9os090.bmp AenV9os091.bmp AenV9os092.bmp AenV9os093.bmp AenV9os094.bmp AenV9os095.bmp 
START_Aen010 =  AenV9os096.bmp AenV9os097.bmp AenV9os098.bmp AenV9os099.bmp AenV9os100.bmp AenV9os101.bmp AenV9os102.bmp AenV9os103.bmp AenV9os104.bmp AenV9os105.bmp 
START_Aen011 =  AenV9os106.bmp AenV9os107.bmp AenV9os108.bmp AenV9os109.bmp AenV9os110.bmp AenV9os111.bmp AenV9os112.bmp AenV9os113.bmp AenV9os114.bmp AenV9os115.bmp 
START_Aen012 =  AenV9os116.bmp AenV9os117.bmp AenV9os118.bmp AenV9os119.bmp AenV9os120.bmp AenV9os121.bmp AenV9os122.bmp AenV9os123.bmp AenV9os124.bmp AenV9os125.bmp 
START_Aen013 =  AenV9os126.bmp AenV9os127.bmp AenV9os128.bmp AenV9os129.bmp AenV9os130.bmp AenV9os131.bmp AenV9os132.bmp AenV9os133.bmp AenV9os134.bmp AenV9os135.bmp 
START_Aen014 =  AenV9os136.bmp AenV9os137.bmp AenV9os138.bmp AenV9os139.bmp AenV9os140.bmp AenV9os141.bmp AenV9os142.bmp AenV9os143.bmp AenV9os144.bmp AenV9os145.bmp 
START_Aen015 =  AenV9os146.bmp AenV9os147.bmp AenV9os148.bmp AenV9os149.bmp AenV9os150.bmp AenV9os151.bmp AenV9os152.bmp AenV9os153.bmp AenV9os154.bmp AenV9os155.bmp 
START_Aen016 =  AenV9os156.bmp AenV9os157.bmp AenV9os158.bmp AenV9os159.bmp AenV9os160.bmp AenV9os161.bmp AenV9os162.bmp AenV9os163.bmp AenV9os164.bmp AenV9os165.bmp 
START_Aen017 =  AenV9os166.bmp AenV9os167.bmp AenV9os168.bmp AenV9os169.bmp AenV9os170.bmp AenV9os171.bmp AenV9os172.bmp AenV9os173.bmp AenV9os174.bmp AenV9os175.bmp 
START_Aen018 =  AenV9os176.bmp AenV9os177.bmp AenV9os178.bmp AenV9os179.bmp AenV9os180.bmp AenV9os181.bmp AenV9os182.bmp AenV9os183.bmp AenV9os184.bmp AenV9os185.bmp 
START_Aen019 =  AenV9os186.bmp AenV9os187.bmp AenV9os188.bmp AenV9os189.bmp AenV9os190.bmp AenV9os191.bmp AenV9os192.bmp AenV9os193.bmp AenV9os194.bmp AenV9os195.bmp 
START_Aen020 =  AenV9os196.bmp AenV9os197.bmp AenV9os198.bmp AenV9os199.bmp AenV9os200.bmp AenV9os201.bmp AenV9os202.bmp AenV9os203.bmp AenV9os204.bmp AenV9os205.bmp 
START_Aen021 =  AenV9os206.bmp AenV9os207.bmp AenV9os208.bmp AenV9os209.bmp AenV9os210.bmp AenV9os211.bmp AenV9os212.bmp AenV9os213.bmp AenV9os214.bmp AenV9os215.bmp 
START_Aen022 =  AenV9os216.bmp AenV9os217.bmp AenV9os218.bmp AenV9os219.bmp AenV9os220.bmp AenV9os221.bmp AenV9os222.bmp AenV9os223.bmp AenV9os224.bmp AenV9os225.bmp 
START_Aen023 =  AenV9os226.bmp AenV9os227.bmp AenV9os228.bmp AenV9os229.bmp AenV9os230.bmp AenV9os231.bmp AenV9os232.bmp AenV9os233.bmp AenV9os234.bmp AenV9os235.bmp 
START_Aen024 =  AenV9os236.bmp AenV9os237.bmp AenV9os238.bmp AenV9os239.bmp AenV9os240.bmp AenV9os241.bmp AenV9os242.bmp AenV9os243.bmp AenV9os244.bmp AenV9os245.bmp 
START_Aen025 =  AenV9os246.bmp AenV9os247.bmp AenV9os248.bmp AenV9os249.bmp AenV9os250.bmp AenV9os251.bmp AenV9os252.bmp AenV9os253.bmp AenV9os254.bmp  



START_LOGO = $(START_Aen001) $(START_Aen002) $(START_Aen003) $(START_Aen004) $(START_Aen005) $(START_Aen006) $(START_Aen007) $(START_Aen008) $(START_Aen009) $(START_Aen010) $(START_Aen011) $(START_Aen012) $(START_Aen013) $(START_Aen014) $(START_Aen015) $(START_Aen016) $(START_Aen017) $(START_Aen018) $(START_Aen019) $(START_Aen020) $(START_Aen021) $(START_Aen022) $(START_Aen023) $(START_Aen024) $(START_Aen025) 
FONTFILE = font.fnt
BUILD_PATH = ./build
EXKNL_PATH = ./exckrnl
DSTPATH = ../exckrnl
RELEDSTPATH = ../release
INITLDR_BUILD_PATH =./initldr/build/
INITLDR_PATH =./initldr/
CPLILDR_PATH =./release/   
INSTALL_PATH =/boot/
INSTALLSRCFILE_PATH =./release/virginos.eki
SRCFILE = $(BOOTEXCIMG) $(KRNLEXCIMG) $(LDEREXCIMG) $(SHELEXCIMG)
RSRCFILE = $(BOOTEXCIMG) $(KRNLEXCIMG) $(LDEREXCIMG) $(SHELEXCIMG) #$(VDIFNAME) $(VMDKFNAME)

INITLDRIMH = initldrimh.bin
INITLDRKRL = initldrkrl.bin
INITLDRSVE = initldrsve.bin

CPLILDRSRC= $(INITLDR_BUILD_PATH)$(INITLDRSVE) $(INITLDR_BUILD_PATH)$(INITLDRKRL) $(INITLDR_BUILD_PATH)$(INITLDRIMH)

LKIMG_INFILE = $(INITLDRSVE) $(INITLDRKRL) $(KRNLEXCIMG) $(FONTFILE) $(LOGOFILE) $(START_LOGO)
.PHONY : install x32 build print clean all krnlexc cpkrnl wrimg phymod exc vdi vdiexc cprelease release createimg

build: clean print all

all:
	$(MAKE) $(X86BARD)
	@echo '恭喜我，系统编译构建完成！ ^_^'
clean:
	$(CD) $(INITLDR_PATH); $(MAKE) clean
	$(CD) $(BUILD_PATH); $(RM) -f *.o *.bin *.i *.krnl *.s *.map *.lib *.btoj *.vdi *.elf *vmdk *.lds *.mk *.mki krnlobjs.mh
	$(CD) $(EXKNL_PATH); $(RM) -f *.o *.bin *.i *.krnl *.s *.map *.lib *.btoj *.vdi *.elf *vmdk *.lds *.mk *.mki krnlobjs.mh
	$(CD) $(CPLILDR_PATH); $(RM) -f *.o *.bin *.i *.krnl *.s *.eki *.map *.lib *.btoj *.elf *.vdi *vmdk *.lds *.mk *.mki krnlobjs.mh
	@echo 'virginOS:清理全部已构建文件... ^_^'

print:
	@echo '*********正在开始编译构建系统*************'


krnlexc: cpkrnl wrimg exc
dbugkrnl: cpkrnl wrimg dbugexc
vdiexc:  vdi
	$(MAKE) $(VBOXVMFLGS)
vdi:cpkrnl wrimg
	$(CD) $(EXKNL_PATH) && $(IMGTOVDI) $(DSKIMG) $(VDIFNAME)
	$(CD) $(EXKNL_PATH) && $(IMGTOVMDK) $(DSKIMG) $(VMDKFNAME)
cplmildr:
	$(CP) $(CPFLAGES) $(CPLILDRSRC) $(CPLILDR_PATH)

cpkrnl:
	$(CD) $(BUILD_PATH) && $(CP) $(CPFLAGES) $(SRCFILE) $(DSTPATH)
cprelease:
	$(CD) $(EXKNL_PATH) && $(CP) $(CPFLAGES) $(RSRCFILE) $(RELEDSTPATH)

exc:
	$(CD) $(EXKNL_PATH) && $(VM) $(VMFLAGES)
dbugexc:
	$(CD) $(EXKNL_PATH) && $(DBUGVM)

KIMG:
	@echo '正在生成virginOS内核映像文件，请稍后……'
	$(CD) $(CPLILDR_PATH) && $(LKIMG) -lhf $(INITLDRIMH) -o virginos.eki -f $(LKIMG_INFILE)

KVMRUN:
	$(VM) $(VMFLAGES)
#$(MAKE) $(KVMRLMOSFLGS)

VBOXRUN:
	$(MAKE) $(VVMRLMOSFLGS)

#cpkrnl cprelease
release: clean all cplmildr cpkrnl cprelease KIMG 

kvmtest: release KVMRUN

vboxtest: release VBOXRUN


createimg:
	$(DD) bs=512 if=/dev/zero of=$(DSKIMG) count=$(IMGSECTNR)

install:
	@echo '下面将开始安装virginOS内核，请确认……'
	sudo $(CP) $(CPFLAGES) $(INSTALLSRCFILE_PATH) $(INSTALL_PATH)
	@echo 'virginOS内核已经安装完成，请重启计算机……'
