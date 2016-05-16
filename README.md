# Slave_basketball_bot_controller
This is the second version of the basketball bot controller(SLAVE).
<h2>第二代篮球机器人底盘STM32F4主控程序</h2>
<p><font size="10px">底盘为三轮全向结构</font></p>
<p>
    <ul>
        <li>main.c内状态机处理来自上位机的串口接收队列消息：</li>
            <ul>
                <li>设置世界坐标系速度</li>
                <li>设置机器人坐标系速度</li>
                <li>读取世界坐标系x,y,w坐标</li>
                <li>重置编码器数据</li>
            </ul>
        <li>TF.c内包含速度解算与里程解算:</li>
            <ul>
                <li>世界坐标系速度解算为三轮速度</li>
                <li>机器人坐标系速度解算为三轮速度</li>
                <li>三轮脉冲速度解算为世界坐标系实际速度，积分得世界坐标系里程坐标</li>
            </ul>
        <li>与STM32F1采用CAN通信，实现里程清零与里程请求</li>
        <li>与三轮电机驱动盒采用CAN通信，实现三轮速度设置</li>
    </ul>
</p>
