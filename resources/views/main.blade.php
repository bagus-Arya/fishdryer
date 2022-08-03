@extends('layouts.mainlayout')
@section('container')

<div class="container py-3">
    <div class="row">
        <div class="col-md-12 col-lg-3">
            <div class="card card text-bg-light my-1">
                <div class="card-header">
                  Device Status
                </div>
                <div class="card-body">
                    <div class="row justify-content-center">
                        <div class="dot-device-status {{ $DeviceInfos->online_status ? 'green' : 'red' }}" id="onOffStatus">
                            @if ($DeviceInfos->online_status)
                                ON
                            @else
                                OFF
                            @endif
                        </div>
                    </div>
                    <div class="row mt-2">
                        <i class="fa-solid fa-calendar-clock"></i>
                        <p class="card-text mb-0 mt-12"><i class="fa-solid fa-calendar-days"></i> Last Request : <span id="lastRequestDate">{{ \Carbon\Carbon::parse($DeviceInfos->updated_at)->format('M d,Y')}}</span></p>
                        <p class="card-text mb-0"><i class="fa-solid fa-clock"></i> Time : <span id="lastRequestTime">{{ \Carbon\Carbon::parse($DeviceInfos->updated_at)->format('H:i')}}</span></p>
                        
                    </div>
                </div>
              </div>
        </div>
        <div class="col-md-12 col-lg-9">
            <div class="card card text-bg-light my-1">
                <div class="card-header">
                  Data Hari ini
                </div>
                <div class="card-body">
                  <div class="container-fluid">
                    <div class="row">
                        <p class="card-text mb-2"><i class="fa-solid fa-calendar-days"></i> Date : <span id="currentDate">{{ \Carbon\Carbon::parse($DateNow)->format('M d,Y') }}</span> </p>
                    </div>
                    <div class="row justify-content-center">
                        <div class="col-sm-6 col-lg-4">
                            <div class="card my-1">
                                <div class="card-header">
                                <i class="fa-solid fa-temperature-high"></i>
                                  Suhu
                                </div>
                                <div class="card-body">
                                    <div class="row justify-content-center">
                                        <div class="dot-device-suhuberat blue" id="currentSuhu">
                                            {{ $todayData->suhu }} C
                                        </div>
                                    </div>
                                </div>
                              </div>
                        </div>
                        <div class="col-sm-6 col-lg-4">
                            <div class="card my-1">
                                <div class="card-header">
                                    <i class="fa-solid fa-weight-scale"></i>
                                  Berat
                                </div>
                                <div class="card-body">
                                    <div class="row justify-content-center">
                                        <div class="dot-device-suhuberat yellow" id="currentBerat">
                                            {{ $todayData->berat }} kg
                                        </div>
                                    </div>
                                </div>
                              </div>
                        </div>
                        <div class="col-sm-6 col-lg-4">
                            <div class="card my-1">
                                <div class="card-header">
                                    <i class="fa-solid fa-lightbulb"></i>
                                  Lampu
                                </div>
                                <div class="card-body">
                                    <div class="row justify-content-center">
                                        <div class="dot-device-suhuberat {{ $DeviceInfos->online_status ? 'green' : 'red' }}" id="currentLampu">
                                            {{ $DeviceInfos->online_status ? 'ON' : 'OFF' }}
                                        </div>
                                    </div>
                                </div>
                              </div>
                        </div>
                        <div class="col-sm-6 d-lg-none">
                            
                        </div>
                    </div>
                    <div class="row">
                       <div class="col-12">
                        <div class="card my-1">
                            <div class="card-header">
                            <i class="fa-solid fa-chart-line"></i>
                              Graph Data
                            </div>
                            <div class="card-body">
                                <div class="row justify-content-center">
                                    {{-- <div>
                                        <canvas id="myChart"></canvas>
                                    </div> --}}
                                    <div class="col-xl-6">
                                        <div class="chart-hariini mt-2">
                                            <canvas id="currentSuhuChart"></canvas>
                                        </div>
                                        
                                    </div>
                                    <div class="col-xl-6">
                                        <div class="chart-hariini mt-2">
                                            <canvas id="currentBeratChart"></canvas>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                       </div>
                    </div>
                  </div>
                </div>
            </div>
        </div>
    </div>
    <div class="row">
        <div class="col-12">
            <div class="card card text-bg-light my-2">
                <div class="card-header">
                  Graph Range
                </div>
                <div class="card-body">
                    <form action="" method="get" class="row">
                        <div class="col-6 col-lg-3">
                            <label for="fromdate" class="form-label">From Date :</label>
                            <input type="date" class="form-control" id="fromdate" value="{{ \Carbon\Carbon::today()->subDays(7)->format('Y-m-d') }}">
                        </div>
                        <div class="col-6 col-lg-3">
                            <label for="todate" class="form-label">To Date</label>
                            <input type="date" class="form-control" id="todate" value="{{ \Carbon\Carbon::today()->format('Y-m-d') }}">
                        </div>
                        <div class="col-12 col-lg-2 align-self-end">
                            <label for="sumbit" class="form-label d-none"></label>
                            <button type="submit" class="btn btn-success w-100 mb-lg-0 mb-3 mt-lg-0 mt-3 text-light">Filter</button>
                        </div>
                    </form>
                    <div class="row mt-2">
                        <div class="row justify-content-center">
                            <div class="col-xl-6">
                                <div class="chart-hariini mt-2">
                                    <canvas id="rangeSuhu"></canvas>
                                </div>
                                
                            </div>
                            <div class="col-xl-6">
                                <div class="chart-hariini mt-2">
                                    <canvas id="rangeBerat"></canvas>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
              </div>
        </div>
    </div>
</div>

<div class="container">
    <footer class="d-flex flex-wrap justify-content-between align-items-center py-3 my-4 border-top">
      <div class="col-md-4 d-flex align-items-center">
        <img src="{{ asset('assests/image/fish.png')}}" class="footer-logo me-1" alt="...">
        <span class="mb-3 mb-md-0 text-muted"> © 2022 Fish Dryer, Inc</span>
      </div>
  
    </footer>
</div>

<script>
    const currentSuhu={
        type: 'line',
        data:{
            datasets: @json($todayGraphSuhu)
        },
        options:{
            responsive:true,
            maintainAspectRatio:false,
            scales: {
                x: {
                    type: 'time',
                    time: {
                        unit:'second'
                    }
                    },
                y:{
                    min:0
                }
            }
        }
    }

    const currentBerat={
        type: 'line',
        data:{
            datasets: @json($todayGraphBerat)
        },
        options:{
            responsive:true,
            maintainAspectRatio:false,
            scales: {
                x: {
                    type: 'time',
                    time: {
                        unit:'second'
                    }
                    },
                y:{
                    min:0
                }
            }
        }
    }

    const rangeSuhu={
        type: 'line',
        data:{
            datasets: @json($rangeGraphSuhu)
        },
        options:{
            responsive:true,
            maintainAspectRatio:false,
            scales: {
                x: {
                    type: 'time',
                    time: {
                        unit:'day'
                    }
                    },
                y:{
                    min:0
                }
            }
        }
    }

    const rangeBerat={
        type: 'line',
        data:{
            datasets: @json($rangeGraphBerat)
        },
        options:{
            responsive:true,
            maintainAspectRatio:false,
            scales: {
                x: {
                    type: 'time',
                    time: {
                        unit:'day'
                    }
                    },
                y:{
                    min:0
                }
            }
        }
    }
</script>
<script>
    const myChart = new Chart(
      document.getElementById('currentSuhuChart'),
      currentSuhu
    );
    const myChart2 = new Chart(
      document.getElementById('currentBeratChart'),
      currentBerat
    );
    const myChart3 = new Chart(
      document.getElementById('rangeSuhu'),
      rangeSuhu
    );
    const myChart4 = new Chart(
      document.getElementById('rangeBerat'),
      rangeBerat
    );
</script>
<script>
    function startLiveUpdate(){
        // DeviceInfo
        TextonOffStatus=document.getElementById('onOffStatus');
        TextlastRequestDate=document.getElementById('lastRequestDate');
        TextlastRequestTime=document.getElementById('lastRequestTime');
        
        // Data Hari ini
        TextcurrentDate=document.getElementById('currentDate');
        TextcurrentSuhu=document.getElementById('currentSuhu');
        TextcurrentBerat=document.getElementById('currentBerat');
        TextcurrentLampu=document.getElementById('currentLampu');
        setInterval(() => {
            fetch('{{ route('livedata') }}')
            .then((response) => response.json())
            .then((data) => console.log(data))
            .catch((error) => console.error('Error:', error));
        }, 2000);
        
    }

    document.addEventListener('DOMContentLoaded',function(){
        startLiveUpdate()
    }) 
</script>
@endsection