-- init code for hwstub_tools

--
-- HELP
--
HELP = hwstub.help

function HELP:create_topic(name)
    self[name] = { create_topic = HELP.create_topic, add = HELP.add, get_topic = HELP.get_topic }
    return self[name]
end

function HELP:get_topic(name)
    return self[name]
end

function HELP:add(text)
    table.insert(self, text)
end

do
    local h = HELP:create_topic("hwstub")
    h:add("This tool uses a number of well-defined namespaces (tables) to organise its features.")
    h:add("The hwstub table contains a number of information and functions related to the tool itself.")
    h:add("Of particular interest are")
    h:add("* hwstub.host which holds host specific information.")
    h:add("* hwstub.dev which holds device specific information. See DEV")
    h:add("* hwstub.help (aka HELP) which holds the help. See HELP.");
    h:add("* hwstub.soc which holds soc specific information. See HW");

    h = HELP:create_topic("HELP");
    h:add("This variable redirects to hwstub.help and provides access to the help system.");
    h:add("You can enhance the help using the following methods on any topic (including HELP itself).");
    h:add("* t:create_topic(s) to create a new subtopic named s under topic t");
    h:add("* t:add(s) to add a help line to topic t");
    h:add("* t:get_topic(s) to get the subtopic s under topic t");

    h = HELP:create_topic("DEV");
    h:add("This variable redirects to hwstub.dev and provides direct access to the device.");
    h:add("It contains some information about the device and the following methods.");
    h:add("* read8/16/32(a) reads a 8/16/32-bit integer at address a");
    h:add("* write8/16/32(a, v) writes the 8/16/32-bit integer v at address a");

    h = HELP:create_topic("HW");
    h:add("This variable redirects to the current soc under hwstub.soc and should be changed by calling hwstub:soc:select only.");
    h:add("The complete register tree can be found under HW in a well organise fashion.");
    h:add("* HW.dev points to device dev");
    h:add("* HW.dev[i] points to device devi if there are several copies of the device at different addresses.");
    h:add("* HW.dev.reg points to the register reg under dev");
    h:add("* HW.dev.reg[i] points to the register regi if there are several copies.");
    h:add("* HW.dev.reg.f points to the field f under register reg.");
    h:add("* HW.dev.reg.f.v gives the value of named value v of field f.");
    h:add("* All registers can be read using HW.dev.reg.read() and written using HW.dev.reg.write(v).");
    h:add("* Register with a SCT variant also implement HW.dev.reg.set/clr/tog(v).");
    h:add("* All register field can be read using HW.dev.reg.f.read() and written using HW.dev.reg.f.write(v).");
    h:add("* Field writes can either give a integer or a named value to write(v).");
    h:add("* Register with a SCT variant also implement HW.dev.reg.f.set/clr/tog(v) with the same properties.");
    h:add("* All devices, registers and fields also have descriptions available such as addresses.");
end

--
-- INFO
--

if not hwstub.options.quiet then
    print("information")
    print("  hwstub")
    print("    version: " .. string.format("%d.%d.%d", hwstub.host.version.major,
        hwstub.host.version.minor, hwstub.host.version.revision))
    print("  device")
    print("    version: " .. string.format("%d.%d.%d", hwstub.dev.version.major,
        hwstub.dev.version.minor, hwstub.dev.version.revision))
    print("    layout")
    print("      on-chip ram")
    print("        code: " .. string.format("%#x bytes @ %#x",
        hwstub.dev.layout.ocram.code.size, hwstub.dev.layout.ocram.code.start))
    print("        stack: " .. string.format("%#x bytes @ %#x",
        hwstub.dev.layout.ocram.stack.size, hwstub.dev.layout.ocram.stack.start))
    print("        buffer: " .. string.format("%#x bytes @ %#x",
        hwstub.dev.layout.ocram.buffer.size, hwstub.dev.layout.ocram.buffer.start))
    print("    features");
    print("      log: " .. tostring(hwstub.dev.features.log))
    print("      mem: " .. tostring(hwstub.dev.features.mem))
    print("      call: " .. tostring(hwstub.dev.features.call))
    print("      jump: " .. tostring(hwstub.dev.features.jump))
    print("      aes_otp: " .. tostring(hwstub.dev.features.aes_otp))
end

--
-- SOC
--
function hwstub.soc:select(soc)
    if self[soc] == nil then return false end
    print("Selecting soc " .. soc .. ". Redirecting HW to hwstub.soc." .. soc)
    HW = self[soc]
    return true
end

--
-- DEV
--
DEV = hwstub.dev

require "lua/load"
