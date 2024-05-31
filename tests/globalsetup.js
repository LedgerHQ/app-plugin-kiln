import Zemu from '@zondax/zemu';
import fsExtra from 'fs-extra';

const catchExit = async () => {
  process.on('SIGINT', () => {
    Zemu.stopAllEmuContainers(function () {
      process.exit();
    });
  });
};

module.exports = async () => {
  console.log('Waited 7 seconds before running tests\n');
  await new Promise((resolve) => setTimeout(resolve, 7000));
  await catchExit();
  await Zemu.checkAndPullImage();
  await Zemu.stopAllEmuContainers();
  fsExtra.emptyDirSync('snapshots/tmp');
};
