/*
 * Project SPIFFS_Test0
 * Description:
 * Author:
 * Date:
 */

 #include "Particle.h"
 #include "spiffs.h"
 #include "Adafruit_TinyFlash.h"

 SYSTEM_MODE(MANUAL);

 // int RequestFile(const char * url, const char * urlPath, const char * destinationFilename);
 void ReceivingFile();

 static spiffs fs;
 #define LOG_PAGE_SIZE       256

 static u8_t spiffs_work_buf[LOG_PAGE_SIZE * 2];
 static u8_t spiffs_fds[32 * 4];
 static u8_t spiffs_cache_buf[(LOG_PAGE_SIZE + 32) * 4];
 Adafruit_TinyFlash flash;

 static void test_spiffs();
 static int32_t my_spiffs_read(uint32_t addr, uint32_t size, uint8_t *dst) {
 	flash.readData(dst, addr, size);
 	return SPIFFS_OK;
 }

 static int32_t my_spiffs_write(uint32_t addr, uint32_t size, uint8_t *src) {
 	if (flash.writePage(addr, src, size))
 		return SPIFFS_OK;
 	return SSPIFFS_WRITE_ERR;
 }

 static int32_t my_spiffs_erase(uint32_t addr, uint32_t size) {
 	if (flash.eraseSector(addr))
 		return SPIFFS_OK;
 	return SPIFFS_ERASE_ERR;
 }


 void my_spiffs_mount() {
 	spiffs_config cfg;
 	// 	cfg.phys_size = 2 * 1024 * 1024; // use all spi flash
 	// 	cfg.phys_addr = 0; // start spiffs at start of spi flash
 	// 	cfg.phys_erase_block = 65536; // according to datasheet
 	// 	cfg.log_block_size = 65536; // let us not complicate things
 	// 	cfg.log_page_size = LOG_PAGE_SIZE; // as we said

 	cfg.hal_read_f = my_spiffs_read;
 	cfg.hal_write_f = my_spiffs_write;
 	cfg.hal_erase_f = my_spiffs_erase;

 	int res = SPIFFS_mount(&fs,
 		&cfg,
 		spiffs_work_buf,
 		spiffs_fds,
 		sizeof(spiffs_fds),
 		spiffs_cache_buf,
 		sizeof(spiffs_cache_buf),
 		0);
 	//Serial.printf("mount res: %i\n", res);
 }

 TCPClient client;
 char server[] = "vectorlogo.biz";
 char path[] = "/wp-content/uploads/2013/01/GOOGLE-VECTORLOGO-BIZ-128x128.png";



 void setup()
 {
   Cellular.off();
   delay(5000);
 	Serial.begin(115200);
 // 	while (!Serial.available())
 // 	{
 // 		Particle.process();
 // 	}
 	Serial.println("Hello SPIFFS");

 	if (flash.begin(D2))
 	{
 		// Flash is compatible
 	}
 	else
 	{
 		// Incorrect model number or unable to communicate with flash
 		Serial.println("Flash init error");
 		while (1);
 	}
 	flash.WriteStatusRegister(0);

 	// Erase all flash. Do this only the first time you use the flash chip!
 	//flash.eraseChip();

 	my_spiffs_mount();

 	test_spiffs();

 	Serial.println("connecting...");
 // 	RequestFile(server, path, "tcpFile");

 }


 // void loop()
 // {
 // 	ReceivingFile();
 //
 // }

 typedef enum
 {
 	ReceptionState_Idle = 0,
 	ReceptionState_ScanHeader,
 	ReceptionState_ScanContentLength,
 	ReceptionState_readData,

 }ReceptionState;

 //#define DEBUG_RECEIVED_DATA(...) Serial.printlnf(__VA_ARGS__)
 #define DEBUG_RECEIVED_DATA(...)

 spiffs_file destinationFile;
 ReceptionState receiveState = ReceptionState_Idle;
 const char * dstFile;

 static void test_spiffs() {
 	char buf[12];

 	// Surely, I've mounted spiffs before entering here

 	spiffs_file fd = SPIFFS_open(&fs, "my_file", SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR, 0);
 	if (SPIFFS_write(&fs, fd, (u8_t *)"Hello world", 12) < 0)
 		Serial.printf("errno %i\n", SPIFFS_errno(&fs));
 	SPIFFS_close(&fs, fd);

 	fd = SPIFFS_open(&fs, "my_file", SPIFFS_RDWR, 0);
 	if (SPIFFS_read(&fs, fd, (u8_t *)buf, 12) < 0)
 		Serial.printf("errno %i\n", SPIFFS_errno(&fs));
 	SPIFFS_close(&fs, fd);

 	Serial.printf("--> %s <--\n", buf);
 	uint32_t total, used;
 	SPIFFS_info(&fs, &total, &used);
 	Serial.printf("Total: %d, Used: %d", total, used);
 }
