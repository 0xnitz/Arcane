// have cleanup mechanism at the end of exection that use apc that iterates over vad, writes over the bytes and unlinks it from the MMVAD_ROOT
// need to have actual entrypoint like LPTHREAD_START_ROUTINE besides the DLLMain
// create compiler script that encrypts this into the out dir