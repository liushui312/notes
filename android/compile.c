linux:
./build.sh config
./build.sh package

android:
source build/envsetup.sh
lunch [combo]
extract-bsp     //复制bImage和modules
make -j8
pack 或 pack -d


make_ota_package
make_ota_package_inc
make_ota_target_file

------------------------------------------------------------------------
在Android源码整编后执行 make otapackage 命令即可生成OTA整包, 步骤如下
1. 重新对system.img文件进行了打包；
2. 生成差分资源包
3. 生成OTA整包, out/target/product/{product_name}/{product_name}-ota-{version-name}.zip

将OTA整包改名为update.zip放到SD卡根目录, 即可开始recovery模式下的 OTA 升级

编译过程出错：
  acp: file 'out/target/product/polaris-p1/bootloader' does not exist
  acp: file 'out/target/product/polaris-p1/bootloader.fex' does not exist
  acp: file 'out/target/product/polaris-p1/env.fex' does not exist
  make: *** [out/target/product/polaris-p1/obj/PACKAGING/target_files_intermediates/polaris-p1-target_files-20130829.zip] Error 1

