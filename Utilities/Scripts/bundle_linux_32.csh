#!/bin/csh -f
#
# this script is called from windows which passes in the directory containing this script
#
set fds_smvroot=~/$1
set makedir=$fds_smvroot/FDS_Compilation
set googledir=$fds_smvroot/Utilities/to_google
set bundlebase=$2
set bundledir=$bundlebase/FDS/FDS5
set bundle_setup=$fds_smvroot/Utilities/Scripts/bundle_setup
set mandir=$fds_smvroot/Manuals/All_PDF_Files
set smvbindir=$fds_smvroot/SMV_5/bin

cd $googledir
rm -rf $bundlebase
mkdir $bundlebase
mkdir $bundlebase/FDS
mkdir $bundledir
mkdir $bundledir/bin
mkdir $bundledir/Documentation
mkdir $bundledir/Examples

echo Copying files
cp $makedir/intel_linux_32/fds5_intel_linux_32 $bundledir/bin/.
cp $makedir/mpi_intel_linux_32/fds5_mpi_intel_linux_32 $bundledir/bin/.
cp $smvbindir/smv5_linux_32 $bundledir/bin/.
cp $smvbindir/smokezip_linux $bundledir/bin/.
cp $smvbindir/smokeview.ini $bundledir/bin/.

cp $bundle_setup/readme_docs.html $bundledir/Documentation/.
cp $mandir/FDS_5_User_Guide.pdf $bundledir/Documentation/.
cp $mandir/SMV_5_User_Guide.pdf $bundledir/Documentation/.

cp $bundle_setup/readme_examples.html $bundledir/Examples/.
svn export --force https://fds-smv.googlecode.com/svn/trunk/FDS/trunk/Verification $bundledir/Examples/.

echo Building archive
rm -rf $googledir/$bundlebase.tar
rm -rf $googledir/$bundlebase.tar.gz
cd $googledir/$bundlebase
tar cvf ../$bundlebase.tar .
echo Compressing archive
gzip    ../$bundlebase.tar
