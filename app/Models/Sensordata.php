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
    // public function getFullNameAttribute()
    // { 
    //   return "{$this->first_name} {$this->last_name}";
    // }
    public function setBeratAttribute($value)
    {
        $this->attributes['berat'] =round($value/1000, 2) ;
    }
    public function setSuhuAttribute($value)
    {
        $this->attributes['suhu'] =round($value, 2) ;
    }
}
