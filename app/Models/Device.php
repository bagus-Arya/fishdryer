<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
Use \Carbon\Carbon;

class Device extends Model
{
    use HasFactory;
    protected $fillable = [
        'api_key',
    ];

     /**
     * The attributes that should be hidden for serialization.
     *
     * @var array<int, string>
     */
    protected $hidden = [
        'api_key',
    ];
     public function getOnlineStatusAttribute()
    { 
        $dateNow = Carbon::now();
      return ($this->updated_at)->diffInSeconds($dateNow)<60 ? True:False;
    }
}
