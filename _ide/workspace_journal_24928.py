# 2026-03-10T14:56:58.126456600
import vitis

client = vitis.create_client()
client.set_workspace(path="TXST_Spring26")

platform = client.create_platform_component(name = "up_SP26",hw_design = "$COMPONENT_LOCATION/../../blackboard.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0",no_boot_bsp = True,compiler = "gcc")

comp = client.create_app_component(name="upLab5",platform = "$COMPONENT_LOCATION/../up_SP26/export/up_SP26/up_SP26.xpfm",domain = "standalone_ps7_cortexa9_0")

platform = client.get_component(name="up_SP26")
status = platform.build()

comp = client.get_component(name="upLab5")
comp.build()

status = platform.build()

comp.build()

vitis.dispose()

