R"(
var contextPanel = $.GetContextPanel();
for (var dnBackground of contextPanel.FindChildrenWithClassTraverse('DeathNoticeBGBorder')) {

	dnBackground.style.border = '0px solid #00000000';
	dnBackground.style.boxShadow = 'inset #00000000 0px 0px 0px 0px;';
	dnBackground.style.borderRadius = '3px';
	dnBackground.style.height = '100%';
	dnBackground.style.width = '100%';
	dnBackground.style.paddingTop = "0px";
	//dnBackground.style.height = '36px';
}

for (var dnBackgroundg of contextPanel.FindChildrenWithClassTraverse('DeathNoticeBG')) {
	dnBackgroundg.style.backgroundImage = 'url("https://cdn.discordapp.com/attachments/755851390496145548/1078939820497305660/1.png")';
	dnBackgroundg.style.backgroundSize = '100% 100%';
	dnBackgroundg.style.backgroundColor = '#00000000';
}

for (var dnBackgroundgradient of contextPanel.FindChildrenWithClassTraverse('DeathNoticeBGGradient')) {
	dnBackgroundgradient.style.height = '100%';
	dnBackgroundgradient.style.width = '95%';
	dnBackgroundgradient.opacity = '0.76';
	dnBackgroundgradient.verticalAlign = 'center';
}

for (var dnIcon of contextPanel.FindChildrenWithClassTraverse('DeathNoticeIcon')) {
	if (dnIcon.id === "Weapon") {
		dnIcon.style.verticalAlign = 'top';
		dnIcon.style.transform = 'translateY(5px)';   
		dnIcon.style.uiScale = "60%";
		dnIcon.style.margin = '-4px 3px 15px 6px';
	} else {
		dnIcon.style.height = '20px';
		dnIcon.style.verticalAlign = 'top';
		dnIcon.style.transform = 'translateY(0px) scaleY(.8) scaleX(.8)';
		dnIcon.style.margin = '-1px 1px 0px 2px'; 
		dnIcon.style.uiScale = "100%";
	}
}

for (var dnT of contextPanel.FindChildrenWithClassTraverse('DeathNoticeTColor')) {
	dnT.style.color = '#d7c788';

}
for (var dnCt of contextPanel.FindChildrenWithClassTraverse('DeathNoticeCTColor')) {
	dnCt.style.color = '#93abd2';

}

for (var deathnotice of contextPanel.FindChildrenWithClassTraverse('DeathNotice')) {
	// ---------------- ICON REMOVALS ----------------
	deathnotice.FindChildTraverse('NoScopeIcon').style.visibility = 'collapse';
	deathnotice.FindChildTraverse('ThroughSmokeIcon').style.visibility = 'collapse';
	deathnotice.FindChildTraverse('AttackerBlindIcon').style.visibility = 'collapse';
	deathnotice.FindChildTraverse('Domination').style.visibility = 'collapse';

	deathnotice.style.margin = '0px -8px 0px 20px';

	if (deathnotice.BHasClass('DeathNotice_Killer')) {
		for (var killer of contextPanel.FindChildTraverse('HudDeathNotice').FindChildrenWithClassTraverse('DeathNotice_Killer')) {
			for (var content of killer.FindChildrenWithClassTraverse('DeathNoticeBG')) {
				content.style.border = '2px solid #b3070d;';
				content.style.borderRadius = '3px';
				content.style.backgroundColor = 'black';
				content.style.opacity = '0.9';
			}
		}
	}
	if (deathnotice.BHasClass('DeathNotice_Victim')) {
		for (var victim of contextPanel.FindChildTraverse('HudDeathNotice').FindChildrenWithClassTraverse('DeathNotice_Victim')) {
			for (var content of victim.FindChildrenWithClassTraverse('DeathNoticeBG')) {

				content.style.backgroundColor = '#0e0e0eB3';
				content.style.borderRadius = '3px';
				content.style.boxShadow = 'inset #e10000e6 0px 0px 1px;';
				content.style.opacity = '1';
			}
		}
	}
}
)"