<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('sensordatas', function (Blueprint $table) {
            $table->id();
            $table->foreignId('device_id')->constrained('devices')->onUpdate('cascade')->onDelete('cascade');
            $table->float('suhu', 8, 2);
            $table->float('berat', 8, 2);
            $table->boolean('lampu');
            $table->timestamps();
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('sensordatas');
    }
};
