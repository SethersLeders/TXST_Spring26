# 2026-03-12T02:21:07.764751300
import vitis

client = vitis.create_client()
client.set_workspace(path="TXST_Spring26")

platform = client.get_component(name="up_SP26")
status = platform.build()

comp = client.get_component(name="upLab4")
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

status = platform.build()

comp.build()

status = platform.build()

comp.build()

vitis.dispose()

