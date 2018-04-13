import pandas
import datetime
import geopy
import geopy.distance
import xml.etree.ElementTree as ET

def GetRainFall(x):
    try:
        return float(x[6][1][0].text)
    except ValueError:
        return 0.05
    

def GetTime(x):
    #2017/1/1 01:00
    time_string=x[0].text
    date,time=time_string.split(" ",1)
    hour,minute=time.split(":",1)
    if hour=="24":
        ret_date=datetime.datetime.strptime(date,"%Y/%m/%d")
        ret_date+=datetime.timedelta(days=1,minutes=int(minute))
        return ret_date
    else:
        return datetime.datetime.strptime(x[0].text,"%Y/%m/%d %H:%M")

def GetID(x):
    return int(x[1].text)

def GethPa(x):
    return float(x[1][1][0].text)

working_dir="D:\\M05A\\"
file_prefix="TDCS_M05A_"
extend_name=".csv"
gate_info_file_name="D:\\etag_gate.csv"
cwb_sta_file_name="D:\\sta_id_loc.csv"
cwb_weather_data_file_name="D:\\C-B0024-001 (1).xml"

gate_info_frame=pandas.read_csv(gate_info_file_name)
cwb_sta_frame=pandas.read_csv(cwb_sta_file_name)
xml_tree=ET.parse(cwb_weather_data_file_name)

data_set_root=xml_tree.getroot().find("{urn:cwb:gov:tw:cwbcommon:0.1}dataset")

cwb_weather_ref_frame=pandas.DataFrame(index=cwb_sta_frame.loc[:,'id'])

for location in data_set_root:
    for location_time in location.findall("{urn:cwb:gov:tw:cwbcommon:0.1}time"):
        cwb_weather_ref_frame.loc[GetID(location),GetTime(location_time)]=location_time
        
print("finish processing weather information")

cur_time=datetime.datetime(2017, 12, 20)
time_step=datetime.timedelta(minutes=5)
end_time=cur_time+datetime.timedelta(weeks=1)
cur_file=working_dir+cur_time.strftime("%Y%m%d")+"\\"+cur_time.strftime("%H")+"\\"+file_prefix+cur_time.strftime("%Y%m%d")+"_"+cur_time.strftime("%H%M%S")+extend_name
cur_frame=pandas.read_csv(cur_file,header=None,names=['time','id1','id2','type','avg','amount'])

using_index=[]
cache_frame=pandas.DataFrame(index=range(0,cur_frame.shape[0]),columns=['direction','miles','latitude','longitude','speed_limit','weather_ref_index'])
cwb_weather_ref_indices=cwb_weather_ref_frame.index
for i in range(0,cur_frame.shape[0]):
    if cur_frame.iloc[i].loc['type']!=31:
        continue
    id1_row=gate_info_frame.loc[gate_info_frame['gate_id'] == cur_frame.iloc[i].loc['id1']].reset_index()
    id2_row=gate_info_frame.loc[gate_info_frame['gate_id'] == cur_frame.iloc[i].loc['id2']].reset_index()
    if id1_row.empty or id2_row.empty:
        continue
    else:
        using_index.append(i)
        cache_frame.iloc[i].loc['direction']=id1_row.iloc[0].loc['direction']
        cache_frame.iloc[i].loc['miles']=(id1_row.iloc[0].loc['miles']+id2_row.iloc[0].loc['miles'])/2
        cache_frame.iloc[i].loc['latitude']=(id1_row.iloc[0].loc['latitude']+id2_row.iloc[0].loc['latitude'])/2
        cache_frame.iloc[i].loc['longitude']=(id1_row.iloc[0].loc['longitude']+id2_row.iloc[0].loc['longitude'])/2
        cache_frame.iloc[i].loc['speed_limit']=(id1_row.iloc[0].loc['speed_limit']+id2_row.iloc[0].loc['speed_limit'])/2
        distance=pandas.Series(index=range(0,cwb_sta_frame.shape[0]))
        gate_loc=(cache_frame.iloc[i].loc['latitude'],cache_frame.iloc[i].loc['longitude'])
        for j in range(0,cwb_sta_frame.shape[0]):
            wea_loc=(cwb_sta_frame.iloc[j,2],cwb_sta_frame.iloc[j,3])
            distance.iloc[j]=geopy.distance.vincenty(gate_loc,wea_loc).kilometers
        cache_frame.iloc[i].loc['weather_ref_index']=cwb_weather_ref_indices.get_loc(cwb_sta_frame.iloc[distance.idxmin(),0])
print("finish processing template frame")
new_row_list=[]
while cur_time<end_time :
    print("finish processing",cur_time)
    cur_file=working_dir+cur_time.strftime("%Y%m%d")+"\\"+cur_time.strftime("%H")+"\\"+file_prefix+cur_time.strftime("%Y%m%d")+"_"+cur_time.strftime("%H%M%S")+extend_name
    cur_frame=pandas.read_csv(cur_file,header=None,names=['time','id1','id2','type','avg','amount'])
    new_day=cur_time.strftime("%w")
    new_time=cur_time.hour*60+cur_time.minute
    for i in using_index:
        if cur_frame.iloc[i,5]==0:
            continue
        new_row_list.append(pandas.Series(index=['direction','miles','latitude','longitude','speed_limit','day','time','rainfall','hpa','speed']))
        #new_row_list[-1].loc['direction']=cache_frame.iloc[i].loc['direction']
        #new_row_list[-1].loc['miles']=cache_frame.iloc[i].loc['miles']
        #new_row_list[-1].loc['latitude']=cache_frame.iloc[i].loc['latitude']
        #new_row_list[-1].loc['longtitude']=cache_frame.iloc[i].loc['longtitude']
        #new_row_list[-1].loc['speed_limit']=cache_frame.iloc[i].loc['speed_limit']
        new_row_list[-1].iloc[0]=cache_frame.iloc[i,0]
        new_row_list[-1].iloc[1]=cache_frame.iloc[i,1]
        new_row_list[-1].iloc[2]=cache_frame.iloc[i,2]
        new_row_list[-1].iloc[3]=cache_frame.iloc[i,3]
        new_row_list[-1].iloc[4]=cache_frame.iloc[i,4]
        new_row_list[-1].iloc[5]=new_day
        new_row_list[-1].iloc[6]=new_time

        cache_frame.iloc[i,5]
        prev_time=cur_time.replace(minute=0)
        after_time=prev_time+datetime.timedelta(hours=1)
        prev_ref=cwb_weather_ref_frame.iloc[cache_frame.iloc[i,5]].loc[prev_time]
        after_ref=cwb_weather_ref_frame.iloc[cache_frame.iloc[i,5]].loc[after_time]
        prev_rainfall=GetRainFall(prev_ref)
        after_rainfall=GetRainFall(after_ref)
        prev_hPa=GethPa(prev_ref)
        after_hPa=GethPa(after_ref)

        after_coef=cur_time.minute
        prev_coef=60-after_coef
        new_row_list[-1].iloc[7]=(prev_rainfall*prev_coef+after_rainfall*after_coef)/60
        new_row_list[-1].iloc[8]=(prev_hPa*prev_coef+after_hPa*after_coef)/60

        new_row_list[-1].iloc[9]=cur_frame.iloc[i].iloc[4]
    cur_time+=time_step
data_frame=pandas.concat(new_row_list,axis=1).T
data_frame.to_csv("D:\\test.csv")
print("finish all")
