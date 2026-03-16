# 2026-03-11T23:41:00.508091600
import vitis

client = vitis.create_client()
client.set_workspace(path="TXST_Spring26")

comp = client.create_app_component(name="upLab4",platform = "$COMPONENT_LOCATION/../up_SP26/export/up_SP26/up_SP26.xpfm",domain = "standalone_ps7_cortexa9_0")

vitis.dispose()

