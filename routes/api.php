<?php

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;
use App\Http\Controllers\SensorDataApiController;
/*
|--------------------------------------------------------------------------
| API Routes
|--------------------------------------------------------------------------
|
| Here is where you can register API routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| is assigned the "api" middleware group. Enjoy building your API!
|
*/

// Route::middleware('auth:sanctum')->get('/user', function (Request $request) {
//     return $request->user();
// });

Route::get('/index/{apikey}',[SensorDataApiController::class,'index']);
Route::get('/show/{apikey}/{sensordata}',[SensorDataApiController::class,'show']);
Route::get('/store/{apikey}/{suhu}/{berat}/{lampu}',[SensorDataApiController::class,'store']);