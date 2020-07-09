var sp = require("serialport");

async function start() {
    let allPorts = await sp.list();
    console.log(allPorts);
    const port = new sp('/dev/tty.wchusbserial1420');
    const parser = sp.parsers.Readline();
    port.pipe(parser);
    parser.on("data", console.log);
}

start();