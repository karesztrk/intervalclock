const char MAIN_page[] = R"=====(
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
    <link rel="icon" href="data:;base64,iVBORw0KGgo=">
    <meta name="viewport" content="width=device-width, initial-scale=1.0" charset="UTF-8">
    <title>Interval timer</title>
    <style>

        html {
            font-family: system-ui,-apple-system,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,"Noto Sans",sans-serif,"Apple Color Emoji","Segoe UI Emoji","Segoe UI Symbol","Noto Color Emoji";
            line-height: 1.5;
        }

        body {
            height: 100vh;
        }

        *, ::after, ::before {
            box-sizing: border-box;
            border-width: 0;
            border-style: solid;
            border-color: #e2e8f0;
        }

        section {
            position: relative;
            color: #4a5568;
        }

        .header {
            color: #4a5568;
            display: flex;
            flex-direction: column;
            margin-bottom: 5rem;
            text-align: center;
            width: 100%;
        }

        h1 {
            text-align: center;
            font-weight: 500;
            margin-bottom: 0.5rem;
            color: #1a202c;
            font-size: 2.25rem;
        }

        .description {
            margin: 0 auto;
            line-height: 1.625;
            font-size: 1rem;
            text-align: center;
        }

        .content {
            color: #4a5568;
            margin: 0 auto;
            width: 100%;
            max-width: 500px;
        }

        @media (min-width: 760px) {
            .content {
                width: calc(100% * 2/3);
            }
        }

        .tab {
            border-color: #667eea;
            border-radius: 0.25rem;
            border-width: 2px;
            display: flex;
            margin: 1.25rem auto 0 auto;
            overflow: hidden;
        }

        .tab-button {
            padding: 0.5rem 1.75rem;
            border-style: none;
            transition-duration: .2s;
            transition-property: all;
            line-height: inherit;
            cursor: pointer;
            background-image: none;
            -webkit-appearance: button;
            text-transform: none;
            overflow: visible;
            font-family: inherit;
            font-size: 100%;
            margin: 0;
            background-color: white;
        }

        .selected-tab {
            color: #fff;
            background-color: #667eea;
        }

        .interval-form {
            margin: -0.5rem;
            flex-wrap: wrap;
            display: flex;
        }

        .form-group {
            width: 100%;
            padding: 0.5rem;
        }

        label {
            color: #4a5568;
            margin-bottom: .5rem;
            font-size: .875rem;
            font-weight: 700;
            display: block;
        }

        .spinner {
            width: 100%;
            position: relative;
            height: 2.5rem;
            justify-content: space-between;
            flex-direction: row;
            display: flex;
            border-width: 1px;
            border-radius: .5rem;
            border-color: #667eea;
        }

        .spinner-button {
            border-style: none;
            padding: 0;
            width: 5rem;
            color: #fff;
            height: 100%;
            font-weight: 600;
            display: flex;
            cursor: pointer;
            border-right-width: 1px;
            border-color: #cbd5e0;
            background-color: #667eea;
            background-image: none;
            -webkit-appearance: button;
            text-transform: none;
            overflow: visible;
            font-family: inherit;
            font-size: 100%;
            line-height: 1.15;
            margin: 0;
            min-width: 40px;
        }

        .decrement-button {
            border-top-left-radius: .25rem;
            border-bottom-left-radius: .25rem;
        }

        .increment-button {
            border-top-right-radius: .25rem;
            border-bottom-right-radius: .25rem;
        }

        .spinner-button span {
            margin: auto;
        }

        .spinner-input {
            color: black;
            font-size: 1rem;
            width: 100%;
            text-align: center;
            cursor: default;
            border-color: #cbd5e0;
            background-color: #fff;
            padding: 0;
            line-height: inherit;
            overflow: visible;
            font-family: inherit;
            margin: 0;
        }

        .start-button {
            margin: 0 0.5rem;
            padding: 0.5rem 2rem;
            border-style: none;
            color: #fff;
            font-size: 1.125rem;
            display: flex;
            border-width: 0;
            border-radius: .25rem;
            background-color: #667eea;
            line-height: inherit;
            cursor: pointer;
            background-image: none;
            -webkit-appearance: button;
            text-transform: none;
            overflow: visible;
            font-family: inherit;
        }

        .stop-button {
            margin: 0 0.5rem;
            padding: 0.5rem 2rem;
            border: solid 2px #667eea;
            font-size: 1.125rem;
            display: flex;
            border-radius: .25rem;
            line-height: inherit;
            cursor: pointer;
            background-image: none;
            -webkit-appearance: button;
            text-transform: none;
            overflow: visible;
            font-family: inherit;
            background-color: white;
        }

        .toast {
            right: 1.25rem;
            top: 1.25rem;
            position: fixed;
        }

        .display-hidden {
            display: none;
        }

        .toast-body {
            box-shadow: 0 10px 15px -3px rgba(0,0,0,.1),0 4px 6px -2px rgba(0,0,0,.05);
            padding: .75rem;
            border-width: 1px;
            border-radius: .5rem;
            border-color: #e2e8f0;
            background-color: #fff;
        }

        .toast-inner {
            display: flex;
            flex-direction: row;
        }

        .toast-icon {
            padding: 0 0.5rem;
        }

        .toast-content {
            margin: 0 1.5rem 0 0.5rem;
        }

        .toast-title {
            font-weight: 600;
        }

        .toast-description {
            color: #a0aec0;
            display: block;
        }

        .action-group {
            display: flex;
            justify-content: center;
            margin-top: 2rem;
        }

    </style>
</head>
<body>

<section>
    <div class="container">
        <div class="header">
            <h1>Interval timer</h1>
            <p class="description">Interval clock timer settings</p>
            <div class="tab">
                <button id="up" class="tab-button selected-tab">Up</button>
                <button id="interval" class="tab-button">Interval</button>
            </div>
        </div>
        <div class="content">

            <div id="interval-form" class="interval-form">
                <div id="prepare-form-group" class="form-group">
                    <label for="prepare">
                        Prepare
                    </label>
                    <div class="spinner">
                        <button id="decrement-prepare" class="spinner-button decrement-button prepare-button">
                            <span>-</span>
                        </button>
                        <input id="prepare" class="spinner-input" name="prepare" value="20" />
                        <button id="increment-prepare" class="spinner-button increment-button prepare-button">
                            <span>+</span>
                        </button>
                    </div>
                </div>
                <div id="work-form-group" class="form-group display-hidden">
                    <label for="work">
                        Work
                    </label>
                    <div class="spinner">
                        <button id="decrement-work" class="spinner-button decrement-button work-button">
                            <span>-</span>
                        </button>
                        <input id="work" class="spinner-input" name="work" value="20" />
                        <button id="increment-work" class="spinner-button increment-button work-button">
                            <span>+</span>
                        </button>
                    </div>
                </div>
                <div id="rest-form-group" class="form-group display-hidden">
                    <label class="block text-gray-700 text-sm font-bold mb-2" for="rest">
                        Rest
                    </label>
                    <div class="spinner">
                        <button class="spinner-button decrement-button rest-button">
                            <span>-</span>
                        </button>
                        <input id="rest" class="spinner-input" name="rest" value="10" />
                        <button class="spinner-button increment-button rest-button">
                            <span>+</span>
                        </button>
                    </div>
                </div>
                <div class="form-group action-group">
                    <button id="start-button" class="start-button">
                        Go!
                    </button>
                    <button id="stop-button" class="stop-button">
                        Stop
                    </button>
                </div>
            </div>
        </div>
    </div>
</section>
<div id="toast" class="toast display-hidden">
    <div class="toast-body">
        <div class="toast-inner">
            <div class="toast-icon">
                <svg width="24" height="24" viewBox="0 0 1792 1792" fill="#44C997" xmlns="http://www.w3.org/2000/svg">
                    <path d="M1299 813l-422 422q-19 19-45 19t-45-19l-294-294q-19-19-19-45t19-45l102-102q19-19 45-19t45 19l147 147 275-275q19-19 45-19t45 19l102 102q19 19 19 45t-19 45zm141 83q0-148-73-273t-198-198-273-73-273 73-198 198-73 273 73 273 198 198 273 73 273-73 198-198 73-273zm224 0q0 209-103 385.5t-279.5 279.5-385.5 103-385.5-103-279.5-279.5-103-385.5 103-385.5 279.5-279.5 385.5-103 385.5 103 279.5 279.5 103 385.5z"/>
                </svg>
            </div>
            <div class="toast-content">
                <span class="toast-title">All set! Starting timer</span>
                <span class="toast-description">The timer is starting, get ready</span>
            </div>
        </div>
    </div>
</div>
<script type="text/javascript">
  document.getElementById('start-button').onclick = sendData;
  document.getElementById('stop-button').onclick = sendStop;

  document.querySelectorAll('.tab-button').forEach(button => {
    button.onclick = switchTab;
  });

  document.querySelectorAll('.decrement-button').forEach(button => {
    if (button.classList.contains('rest-button')) {
      button.onclick = () => decrement('rest');
    } else if (button.classList.contains('work-button')) {
      button.onclick = () => decrement('work');
    } else if (button.classList.contains('prepare-button')) {
      button.onclick = () => decrement('prepare');
    }
  });

  document.querySelectorAll('.increment-button').forEach(button => {
    if (button.classList.contains('rest-button')) {
      button.onclick = () => increment('rest');
    } else if (button.classList.contains('work-button')) {
      button.onclick = () => increment('work');
    } else if (button.classList.contains('prepare-button')) {
      button.onclick = () => increment('prepare');
    }
  });

  function decrement(target) {
    const input = document.getElementById(target);
    if (input) {
      input.value = input.value === '0' ? 0 : Number(input.value) - 1;
    }
  }

  function increment(target) {
    const input = document.getElementById(target);
    if (input) {
      input.value = Number(input.value) + 1;
    }
  }

  function switchTab (event) {
    const tab = event.target.id;
    const tabButton = document.getElementById(tab);

    const workGroup = document.getElementById('work-form-group');
    const restGroup = document.getElementById('rest-form-group');

    const buttons = document.querySelectorAll('.tab-button');
    buttons.forEach(button => button.classList.remove('selected-tab'));
    tabButton.classList.add('selected-tab');

    if (tab === 'up') {
      workGroup.classList.add('display-hidden');
      restGroup.classList.add('display-hidden');
    } else if (tab === 'interval') {
      workGroup.classList.remove('display-hidden');
      restGroup.classList.remove('display-hidden');
    }
  }

  async function post(url, data = {}) {
    return await fetch(url, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/x-www-form-urlencoded',
      },
      body: data
    });
  }

  function sendData() {
    const prepare = document.getElementById('prepare');
    const tab = document.querySelector('.selected-tab');
    const data = new URLSearchParams();

    if (prepare) {
      data.set('prepare', prepare.value);
    }

    if (tab && tab.id === 'interval') {
      const work = document.getElementById('work');
      if (work) {
        data.set('work', work.value);
      }
      const rest = document.getElementById('rest');
      if (rest) {
        data.set('rest', rest.value);
      }
    }

    post('/start', data).then(data => {
      showSuccessToast();
      console.log(data);
    }).catch((error) => {
      console.error(error);
    });

  }

  function sendStop() {
    post('/stop');
  }

  function showSuccessToast() {
    const toast = document.getElementById('toast');
    toast.classList.remove('display-hidden');
    setInterval(() => toast.classList.add('display-hidden'), 5000);
  }

</script>
</body>
</html>
)=====";
