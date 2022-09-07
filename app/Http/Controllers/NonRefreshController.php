<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use \App\Models\Device;
use \App\Models\Sensordata;
Use \Carbon\Carbon;


class NonRefreshController extends Controller
{
    public function livedata(){
        $DeviceInfos=Device::find(1);
        $DeviceInfos['online_status']=$DeviceInfos->online_status;
        $DateNow=Carbon::now();

        $todayData=Sensordata::whereDate('created_at', Carbon::today())->orderBy('created_at','DESC')->first();
        if(!$todayData){
            $todayData=[
                "suhu"=> 'NaN',
                "berat"=> 'NaN',
                "lampu"=> 'NaN',
            ];
            $todayData=(object)$todayData;
        }

        $todayGraphDatas=Sensordata::whereDate('created_at', Carbon::today())->orderBy('created_at','DESC')->limit(10)->get();
        $todayGraphSuhuDatas=[];
        $todayGraphBeratDatas=[];
        foreach ($todayGraphDatas as $todayGraphData){
            array_push($todayGraphSuhuDatas,[
                'x'=>$todayGraphData->created_at,
                'y'=>$todayGraphData->suhu
            ]);
            array_push($todayGraphBeratDatas,[
                'x'=>$todayGraphData->created_at,
                'y'=>$todayGraphData->berat
            ]);
        }

        $todayGraphSuhu=[
            "label"=>"Suhu",
            "backgroundcolor"=>"rgb(13,110,253)",
            "borderColor"=>"rgb(13,110,253)",
            "data"=>$todayGraphSuhuDatas
        ];

        $todayGraphBerat=[
            "label"=>"Berat",
            "backgroundcolor"=>"rgb(255,193,7)",
            "borderColor"=>"rgb(255,193,7)",
            "data"=>$todayGraphBeratDatas
        ];


        return [
            'DeviceInfos'=>$DeviceInfos,
            'DateNow'=>$DateNow,
            'todayData'=>$todayData,
            'todayGraphSuhu'=>$todayGraphSuhu,
            'todayGraphBerat'=>$todayGraphBerat
        ];
    }

    public function livesearch(Request $request){
        try {
            $request['from_date']=\Carbon\Carbon::parse($request['from_date'])->format('Y-m-d');
            $request['to_date']=\Carbon\Carbon::parse($request['to_date'])->format('Y-m-d');
          
            $rangeGraphSuhuDatas=[];
            $rangeGraphBeratDatas=[];
            $from_date= $request['from_date'];
            $to_date=$request['to_date'];

            $rangeGraphDatas=Sensordata::whereDate('created_at', '>=', $from_date)
            ->whereDate('created_at', '<=', $to_date)->get();

            while (strtotime($from_date) <= strtotime($to_date)){
                $fecthDataSensor=Sensordata::whereDate('created_at',$from_date)->get();
                $averageSuhu=0;
                $averageBerat=0;
                foreach($fecthDataSensor as $key=>$dataSensor){
                    $averageSuhu+=$dataSensor->suhu;
                    $averageBerat+=$dataSensor->berat;
                    if( $key+1==$fecthDataSensor->count() ) {
                        $averageSuhu=$averageSuhu/$fecthDataSensor->count();
                        $averageBerat=$averageBerat/$fecthDataSensor->count();
                    }
                }
                array_push($rangeGraphSuhuDatas,[
                    'x'=>$from_date,
                    'y'=>$averageSuhu
                ]);
                array_push($rangeGraphBeratDatas,[
                    'x'=>$from_date,
                    'y'=>$averageBerat
                ]);
                $from_date = date ("Y-m-d", strtotime("+1 days", strtotime($from_date)));
            }

            $rangeGraphSuhu=[
                "label"=>"Suhu",
                "backgroundcolor"=>"rgb(13,110,253)",
                "borderColor"=>"rgb(13,110,253)",
                "data"=>$rangeGraphSuhuDatas
            ];

            $rangeGraphBerat=[
                "label"=>"Berat",
                "backgroundcolor"=>"rgb(255,193,7)",
                "borderColor"=>"rgb(255,193,7)",
                "data"=>$rangeGraphBeratDatas
            ];
            // return ['message'=>'hi'];
            return [
                'rangeGraphSuhu'=>$rangeGraphSuhu,
                'rangeGraphBerat'=>$rangeGraphBerat
            ];
        } catch (\Throwable $th) {
            return abort(500);
        }
    }
}
