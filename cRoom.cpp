#include "cRoom.h"
#include "cRooms.h"

double cRoom::getSpTemp(void)
{
	DayTypes DayType = getDayType( TimeNow.dayOfWeek() );
	
	TimeSpan rel;
	rel.set(0, TimeNow.hour(), TimeNow.minute(), 0);
	
	double MasterSPTemp = 0.0;
	// Check if manual override is valid
	if((SpTempOverride.get()<TempRoomHigh)&&(SpTempOverride.get()>TempRoomLow))
		MasterSPTemp = SpTempOverride.get();
	else
		MasterSPTemp = Rooms->RoomTemps[(RoomType)];
	
	double SpTemp = MasterSPTemp;
	for(int iSwitch=0; iSwitch<nSwitch; iSwitch++)
	{	// Apply offset according to schedule
		if (Rooms->TempOffsetSchedule[Rooms->SetType][RoomType][DayType][iSwitch].time.totalseconds() < rel.totalseconds())
			SpTemp = MasterSPTemp + Rooms->TempOffsetSchedule[Rooms->SetType][RoomType][DayType][iSwitch].temp;
	}
	// Check Limits
	SpTemp = min(max(SpTemp,TempRoomLow),TempRoomHigh);
	return SpTemp;
}