# VMDETECT

vmdetect is a node module for detect virtusl machine envirement.

## Start

```javascript
const vmdetect = require("../build/Release/vmdetect");
console.log(vmdetect.hyperVenderId());
console.log(vmdetect.isHyper());
```

## API

### hyperVenderId()

get hypervisor Vender Id

### isHyper()

check this meachine is hypervisor or not

## License

WTFPL