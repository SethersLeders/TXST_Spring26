# 2026-03-10T15:37:41.192838800
import vitis

client = vitis.create_client()
client.set_workspace(path="TXST_Spring26")

platform = client.get_component(name="up_SP26")
status = platform.build()

comp = client.get_component(name="upLab5")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

vitis.dispose()

