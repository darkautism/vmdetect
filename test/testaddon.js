const vmdetect = require("../build/Release/vmdetect");
console.log(vmdetect.hyperVenderId());
console.log(vmdetect.isHyper());
