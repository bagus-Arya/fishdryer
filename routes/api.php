<?php

use Illuminate\Http\Request;
use Illuminate\Support\Facades\Route;
use App\Http\Controllers\SensorDataApiController;
use App\Http\Controllers\NonRefreshController;
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

Route::get('/index',[SensorDataApiController::class,'index']);
Route::get('/showlatest',[SensorDataApiController::class,'showlatest']);
Route::get('/show/{sensordata}',[SensorDataApiController::class,'show']);
Route::get('/livedata',[NonRefreshController::class,'livedata'])->name('livedata');
Route::get('/livesearch',[NonRefreshController::class,'livesearch'])->name('livesearch');
Route::get('/store/{apikey}/{suhu}/{berat}/{lampu}',[SensorDataApiController::class,'store']);