<?php

namespace App\Http\Controllers;
use Illuminate\Support\Facades\Validator;
use Illuminate\Validation\Rule;
use Illuminate\Support\Facades\Http;

use Illuminate\Http\Request;

class BypassTls extends Controller
{
    public function index(Request $request){
        $validate = $request->validate([
            'url'=>'required|url',
            'dump'=>'required',
        ]);
        $response = Http::acceptJson()->post($validate["url"], $validate["dump"])->json();
        return $response;
    }
}
