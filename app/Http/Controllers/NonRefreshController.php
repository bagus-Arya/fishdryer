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

        $todayGraphSuhu=[[
            "label"=>"Suhu",
            "backgroundcolor"=>"rgb(13,110,253)",
            "borderColor"=>"rgb(13,110,253)",
            "data"=>$todayGraphSuhuDatas
        ]];

        $todayGraphBerat=[[
            "label"=>"Berat",
            "backgroundcolor"=>"rgb(255,193,7)",
            "borderColor"=>"rgb(255,193,7)",
            "data"=>$todayGraphBeratDatas
        ]];


        return [
            'DeviceInfos'=>$DeviceInfos,
            'DateNow'=>$DateNow,
            'todayGraphSuhu'=>$todayGraphBerat,
            'todayGraphBerat'=>$todayGraphBerat
        ];
    }
}
