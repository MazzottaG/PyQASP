input = """
consists_of(computer,mainboard).
consists_of(computer,power_unit).
consists_of(computer,io_unit).
consists_of(mainboard,processor).
consists_of(mainboard,memory_unit).
consists_of(memory_unit,memory_chip).
consists_of(io_unit,disk_unit).
consists_of(io_unit,graphics_card).
consists_of(io_unit,serial_card).
consists_of(disk_unit,disk).
consists_of(disk_unit,controller).
consists_of(controller,controller_chip).
"""
output = """
{consists_of(computer,io_unit), consists_of(computer,mainboard), consists_of(computer,power_unit), consists_of(controller,controller_chip), consists_of(disk_unit,controller), consists_of(disk_unit,disk), consists_of(io_unit,disk_unit), consists_of(io_unit,graphics_card), consists_of(io_unit,serial_card), consists_of(mainboard,memory_unit), consists_of(mainboard,processor), consists_of(memory_unit,memory_chip)}
"""
