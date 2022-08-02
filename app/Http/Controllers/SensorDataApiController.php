<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use \App\Models\Device;
use \App\Models\Sensordata;
use Illuminate\Support\Facades\Validator;
use Illuminate\Validation\Rule;

class SensorDataApiController extends Controller
{
    public function index($apikey){
        $DeviceId=Device::where('api_key',$apikey)->first()->id;
        if(!$DeviceId){
            return response()->json(['message' => 'unautenticated'], 401);
        }
        return Sensordata::where('device_id',$DeviceId)->orderBy('created_at', 'asc')
        ->simplePaginate(10)->withQueryString();
    }

    public function show($apikey,Sensordata $sensordata){
        $DeviceId=Device::where('api_key',$apikey)->first()->id;
        if(!$DeviceId){
            return response()->json(['message' => 'unautenticated'], 401);
        }
        return Sensordata::where('device_id',$DeviceId)->where('id',$sensordata->id)->firstOrFail();
    }

    public function store($apikey,$suhu,$berat,$lampu){
        // $validate = $request->validate([
        //     'suhu'=>'required|numeric',
        //     'berat'=>'required|numeric',
        //     'lampu'=>'required|boolean',
        // ]);
        // 
        // 

        if((is_numeric($suhu) && is_numeric($berat) && ($lampu==1 || $lampu==0))!=True){
            return response()->json(['message' => 'bad request'], 403);
        }


        $DeviceId=Device::where('api_key',$apikey)->first()->id;
        if(!$DeviceId){
            return response()->json(['message' => 'unautenticated'], 401);
        }
        
        return Sensordata::create([
            'device_id'=>$DeviceId,
            'suhu'=>$suhu,
            'berat'=>$berat,
            'lampu'=>$lampu,
        ]);    
    }
}
