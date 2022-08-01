<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Sensordata extends Model
{
    use HasFactory;
    protected $fillable = [
        'suhu',
        'berat',
        'lampu',
        'device_id',
    ];
}